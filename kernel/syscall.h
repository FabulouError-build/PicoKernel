// 系统调用头文件

#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>
#include <stddef.h>

// 系统调用号
typedef enum {
    SYS_exit = 0,        // 退出进程
    SYS_fork = 1,        // 创建进程
    SYS_wait = 2,        // 等待进程
    SYS_read = 3,        // 读取文件
    SYS_write = 4,       // 写入文件
    SYS_open = 5,        // 打开文件
    SYS_close = 6,       // 关闭文件
    SYS_malloc = 7,      // 分配内存
    SYS_free = 8,        // 释放内存
    SYS_getpid = 9,      // 获取进程ID
    SYS_sleep = 10,      // 睡眠
    SYS_gettime = 11,     // 获取时间
    SYS_num = 12         // 系统调用数量
} syscall_num_t;

// 系统调用处理函数类型
typedef int (*syscall_handler_t)(int, int, int, int, int);

// 系统调用初始化
void syscall_init(void);

// 系统调用处理函数
int syscall_handler(int num, int arg1, int arg2, int arg3, int arg4, int arg5);

// 系统调用表
extern syscall_handler_t syscall_table[];

// 用户空间系统调用接口
int syscall(int num, int arg1, int arg2, int arg3, int arg4, int arg5);

// 具体系统调用函数
extern int sys_exit(int status);
extern int sys_fork(void);
extern int sys_wait(int pid);
extern int sys_read(int fd, void *buf, int count);
extern int sys_write(int fd, const void *buf, int count);
extern int sys_open(const char *path, int flags, int mode);
extern int sys_close(int fd);
extern void *sys_malloc(int size);
extern void sys_free(void *ptr);
extern int sys_getpid(void);
extern int sys_sleep(int seconds);
extern int sys_gettime(void);

#endif // SYSCALL_H