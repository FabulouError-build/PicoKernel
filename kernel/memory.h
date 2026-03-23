// 内存管理头文件

#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

// 内存页大小
#define PAGE_SIZE 4096

// 物理内存块结构
typedef struct memory_block {
    uint32_t size;      // 块大小
    uint32_t address;   // 物理地址
    uint8_t free;       // 是否空闲
    struct memory_block *next; // 下一个块
} memory_block_t;

// 内存管理初始化
void memory_init(uint32_t memory_size);

// 分配物理内存
void *malloc(uint32_t size);

// 释放物理内存
void free(void *ptr);

// 获取内存使用情况
void memory_status(void);

#endif // MEMORY_H