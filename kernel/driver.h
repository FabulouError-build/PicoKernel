// 驱动程序头文件

#ifndef DRIVER_H
#define DRIVER_H

#include <stdint.h>
#include <stddef.h>

// 键盘扫描码
#define KEY_BACKSPACE 0x0E
#define KEY_ENTER 0x1C
#define KEY_ESC 0x01
#define KEY_SPACE 0x39

// 颜色定义
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_LIGHT_GRAY 7
#define COLOR_DARK_GRAY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 10
#define COLOR_LIGHT_CYAN 11
#define COLOR_LIGHT_RED 12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

// 驱动程序初始化
void driver_init(void);

// 键盘驱动
void keyboard_init(void);
char keyboard_getch(void);
int keyboard_available(void);

// 时钟驱动
void clock_init(void);
uint32_t get_ticks(void);
void sleep(uint32_t seconds);

// 屏幕驱动
void screen_init(void);
void clear_screen(void);
void put_char(char c, int x, int y, char color);
void put_string(const char *str, int x, int y, char color);
void set_cursor(int x, int y);
int get_cursor_x(void);
int get_cursor_y(void);

// 桌面环境
void desktop_init(void);
void desktop_draw(void);
void desktop_handle_input(char c);

#endif // DRIVER_H