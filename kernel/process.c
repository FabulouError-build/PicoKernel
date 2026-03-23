// 进程管理实现

#include "process.h"
#include "memory.h"

// 外部汇编函数
extern void _process_switch_asm(process_t *next);

// 进程链表头
static process_t *process_list = NULL;

// 当前进程
process_t *_current_process = NULL;

// 下一个进程ID
static uint32_t next_pid = 1;

// 进程管理初始化
void process_init(void) {
    // 创建空闲任务（idle task）
    process_create("idle", NULL, 0);
    
    // 设置当前进程为空闲任务
    _current_process = process_list;
    _current_process->state = PROCESS_RUNNING;
}

// 创建新进程
process_t *process_create(const char *name, void (*entry)(void), uint32_t priority) {
    // 分配进程控制块
    process_t *process = (process_t *)malloc(sizeof(process_t));
    if (!process) {
        return NULL;
    }
    
    // 初始化进程控制块
    process->pid = next_pid++;
    
    // 复制进程名称
    int i = 0;
    while (name[i] && i < 31) {
        process->name[i] = name[i];
        i++;
    }
    process->name[i] = '\0';
    
    process->state = PROCESS_READY;
    process->priority = priority;
    process->page_directory = 0;
    
    // 分配进程栈
    uint32_t stack = (uint32_t)malloc(4096);
    if (!stack) {
        free(process);
        return NULL;
    }
    
    // 初始化栈
    stack += 4096; // 栈顶
    stack &= 0xFFFFFFFC; // 对齐到4字节
    
    // 设置栈帧
    if (entry) {
        // 压入返回地址
        stack -= 4;
        *(uint32_t *)stack = (uint32_t)entry;
        
        // 压入EBP
        stack -= 4;
        *(uint32_t *)stack = 0;
        
        // 压入通用寄存器
        for (int i = 0; i < 8; i++) {
            stack -= 4;
            *(uint32_t *)stack = 0;
        }
    }
    
    process->esp = stack;
    process->eip = entry ? (uint32_t)entry : 0;
    
    // 添加到进程链表
    if (!process_list) {
        process_list = process;
        process->next = process;
        process->prev = process;
    } else {
        process->next = process_list;
        process->prev = process_list->prev;
        process_list->prev->next = process;
        process_list->prev = process;
    }
    
    return process;
}

// 切换进程
void process_switch(process_t *next) {
    if (!next || next == _current_process) {
        return;
    }
    
    // 更新当前进程状态
    if (_current_process->state == PROCESS_RUNNING) {
        _current_process->state = PROCESS_READY;
    }
    
    // 更新下一个进程状态
    next->state = PROCESS_RUNNING;
    
    // 调用汇编实现的进程切换
    _process_switch_asm(next);
}

// 调度进程
void schedule(void) {
    if (!process_list) {
        return;
    }
    
    process_t *best_process = NULL;
    process_t *current = process_list;
    
    // 查找优先级最高的就绪进程
    do {
        if (current->state == PROCESS_READY) {
            if (!best_process || current->priority > best_process->priority) {
                best_process = current;
            }
        }
        current = current->next;
    } while (current != process_list);
    
    // 如果找到合适的进程，切换到该进程
    if (best_process) {
        process_switch(best_process);
    }
}

// 获取当前进程
process_t *get_current_process(void) {
    return _current_process;
}

// 终止进程
void process_exit(void) {
    if (!_current_process) {
        return;
    }
    
    // 标记进程为终止状态
    _current_process->state = PROCESS_TERMINATED;
    
    // 释放进程资源
    // 这里需要释放进程栈等资源
    
    // 从进程链表中移除
    if (_current_process->next == _current_process) {
        // 只有一个进程
        process_list = NULL;
    } else {
        _current_process->prev->next = _current_process->next;
        _current_process->next->prev = _current_process->prev;
        if (process_list == _current_process) {
            process_list = _current_process->next;
        }
    }
    
    // 保存要删除的进程
    process_t *to_delete = _current_process;
    
    // 调度到下一个进程
    schedule();
    
    // 释放进程控制块
    free(to_delete);
}

// 阻塞进程
void process_block(void) {
    if (!_current_process) {
        return;
    }
    
    _current_process->state = PROCESS_BLOCKED;
    schedule();
}

// 唤醒进程
void process_wakeup(process_t *process) {
    if (!process) {
        return;
    }
    
    if (process->state == PROCESS_BLOCKED) {
        process->state = PROCESS_READY;
    }
}
