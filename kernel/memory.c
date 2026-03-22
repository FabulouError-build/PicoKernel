// 内存管理实现

#include "memory.h"

// 内存块链表头
static memory_block_t *memory_blocks = NULL;

// 内存管理初始化
void memory_init(uint32_t memory_size) {
    // 从1MB开始分配内存（跳过低内存区域）
    uint32_t start_address = 0x100000;
    
    // 创建第一个内存块
    memory_blocks = (memory_block_t *)start_address;
    memory_blocks->size = memory_size - start_address;
    memory_blocks->address = start_address + sizeof(memory_block_t);
    memory_blocks->free = 1;
    memory_blocks->next = NULL;
}

// 分配物理内存
void *malloc(uint32_t size) {
    // 对齐到4字节
    size = (size + 3) & ~3;
    
    memory_block_t *current = memory_blocks;
    memory_block_t *prev = NULL;
    
    while (current) {
        if (current->free && current->size >= size) {
            // 检查是否需要分割块
            if (current->size > size + sizeof(memory_block_t)) {
                // 创建新块
                memory_block_t *new_block = (memory_block_t *)(current->address + size);
                new_block->size = current->size - size - sizeof(memory_block_t);
                new_block->address = new_block + 1;
                new_block->free = 1;
                new_block->next = current->next;
                
                // 更新当前块
                current->size = size;
                current->next = new_block;
            }
            
            // 标记为已使用
            current->free = 0;
            return (void *)current->address;
        }
        
        prev = current;
        current = current->next;
    }
    
    // 没有找到足够的内存
    return NULL;
}

// 释放物理内存
void free(void *ptr) {
    if (!ptr) return;
    
    memory_block_t *block = (memory_block_t *)((uint32_t)ptr - sizeof(memory_block_t));
    block->free = 1;
    
    // 合并相邻的空闲块
    memory_block_t *current = memory_blocks;
    while (current) {
        if (current->free && current->next && current->next->free) {
            // 合并块
            current->size += current->next->size + sizeof(memory_block_t);
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

// 获取内存使用情况
void memory_status(void) {
    memory_block_t *current = memory_blocks;
    uint32_t total_free = 0;
    uint32_t total_used = 0;
    int block_count = 0;
    
    while (current) {
        block_count++;
        if (current->free) {
            total_free += current->size;
        } else {
            total_used += current->size;
        }
        current = current->next;
    }
    
    // 这里可以添加打印函数，显示内存使用情况
    // 例如：print_string("Memory Status:", 10, 16, 0x0f);
    //       print_string("Total free: ", 10, 17, 0x0f);
    //       打印数值...
}