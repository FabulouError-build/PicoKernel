// 驱动程序实现

#include "driver.h"

// 全局变量
static int cursor_x = 0;
static int cursor_y = 0;
static uint32_t ticks = 0;
static char keyboard_buffer[256];
static int keyboard_head = 0;
static int keyboard_tail = 0;

// 端口操作函数
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "dN"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

// 驱动程序初始化
void driver_init(void) {
    screen_init();
    keyboard_init();
    clock_init();
    desktop_init();
}

// 屏幕驱动
void screen_init(void) {
    clear_screen();
    set_cursor(0, 0);
}

void clear_screen(void) {
    char *video_memory = (char *)0xb8000;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i + 1] = 0x07; // 黑底白字
    }
    cursor_x = 0;
    cursor_y = 0;
    set_cursor(0, 0);
}

void put_char(char c, int x, int y, char color) {
    if (x < 0 || x >= 80 || y < 0 || y >= 25) {
        return;
    }
    char *video_memory = (char *)0xb8000;
    int offset = (y * 80 + x) * 2;
    video_memory[offset] = c;
    video_memory[offset + 1] = color;
}

void put_string(const char *str, int x, int y, char color) {
    int i = 0;
    while (str[i] && x < 80) {
        put_char(str[i], x, y, color);
        x++;
        i++;
    }
}

void set_cursor(int x, int y) {
    if (x < 0) x = 0;
    if (x >= 80) x = 79;
    if (y < 0) y = 0;
    if (y >= 25) y = 24;
    
    cursor_x = x;
    cursor_y = y;
    
    uint16_t pos = y * 80 + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

int get_cursor_x(void) {
    return cursor_x;
}

int get_cursor_y(void) {
    return cursor_y;
}

// 时钟驱动
void clock_init(void) {
    // 这里需要设置时钟中断
    // 暂时只初始化ticks变量
    ticks = 0;
}

uint32_t get_ticks(void) {
    return ticks;
}

void sleep(uint32_t seconds) {
    uint32_t start = get_ticks();
    while (get_ticks() - start < seconds * 18) {
        // 忙等待
    }
}

// 键盘驱动
void keyboard_init(void) {
    // 这里需要设置键盘中断
    // 暂时只初始化缓冲区
    keyboard_head = 0;
    keyboard_tail = 0;
}

char keyboard_getch(void) {
    if (keyboard_head == keyboard_tail) {
        return 0;
    }
    char c = keyboard_buffer[keyboard_tail];
    keyboard_tail = (keyboard_tail + 1) % 256;
    return c;
}

int keyboard_available(void) {
    return keyboard_head != keyboard_tail;
}

// 桌面环境
void desktop_init(void) {
    clear_screen();
    put_string("PicoKernel Desktop", 30, 0, COLOR_LIGHT_BLUE);
    put_string("=================", 30, 1, COLOR_LIGHT_BLUE);
    put_string("Welcome to PicoKernel!", 25, 3, COLOR_WHITE);
    put_string("Press any key to continue...", 25, 5, COLOR_LIGHT_GRAY);
}

void desktop_draw(void) {
    // 绘制任务栏
    for (int x = 0; x < 80; x++) {
        put_char(' ', x, 24, COLOR_BLUE);
    }
    put_string("PicoKernel v0.1", 0, 24, COLOR_LIGHT_BLUE);
    put_string("Taskbar", 35, 24, COLOR_LIGHT_BLUE);
}

void desktop_handle_input(char c) {
    if (c == KEY_ESC) {
        clear_screen();
        desktop_init();
    } else if (c == KEY_ENTER) {
        cursor_y++;
        cursor_x = 0;
        set_cursor(cursor_x, cursor_y);
    } else if (c == KEY_BACKSPACE) {
        if (cursor_x > 0) {
            cursor_x--;
        } else if (cursor_y > 0) {
            cursor_y--;
            cursor_x = 79;
        }
        put_char(' ', cursor_x, cursor_y, 0x07);
        set_cursor(cursor_x, cursor_y);
    } else if (c >= 32 && c <= 126) {
        put_char(c, cursor_x, cursor_y, 0x07);
        cursor_x++;
        if (cursor_x >= 80) {
            cursor_x = 0;
            cursor_y++;
        }
        if (cursor_y >= 24) {
            clear_screen();
            cursor_x = 0;
            cursor_y = 0;
        }
        set_cursor(cursor_x, cursor_y);
    }
    desktop_draw();
}
