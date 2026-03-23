// 进程管理头文件

#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stddef.h>

// 进程状态
typedef enum {
    PROCESS_RUNNING,
    PROCESS_READY,
    PROCESS_BLOCKED,
    PROCESS_TERMINATED
} process_state_t;

// 进程控制块(PCB)
typedef struct process {
    uint32_t pid;             // 进程ID
    char name[32];           // 进程名称
    process_state_t state;    // 进程状态
    uint32_t priority;        // 进程优先级
    uint32_t esp;             // 栈指针
    uint32_t eip;             // 指令指针
    uint32_t page_directory;  // 页目录地址（用于虚拟内存）
    struct process *next;     // 下一个进程
    struct process *prev;     // 上一个进程
} process_t;

// 进程管理初始化
void process_init(void);

// 创建新进程
process_t *process_create(const char *name, void (*entry)(void), uint32_t priority);

// 切换进程
void process_switch(process_t *next);

// 调度进程
void schedule(void);

// 获取当前进程
process_t *get_current_process(void);

// 终止进程
void process_exit(void);

// 阻塞进程
void process_block(void);

// 唤醒进程
void process_wakeup(process_t *process);

#endif // PROCESS_H