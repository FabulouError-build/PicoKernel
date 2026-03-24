// 内核代码
// 编译: gcc -m32 -nostdlib -nostartfiles -ffreestanding -c kernel.c -o kernel.o

// 屏幕输出函数
void print_char(char c, int x, int y, char color) {
    char *video_memory = (char *)0xb8000;
    int offset = (y * 80 + x) * 2;
    video_memory[offset] = c;
    video_memory[offset + 1] = color;
}

void print_string(const char *str, int x, int y, char color) {
    while (*str) {
        print_char(*str++, x++, y, color);
        if (x >= 80) {
            x = 0;
            y++;
        }
    }
}

// 内核主函数
void kernel_main() {
    // 清屏
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            print_char(' ', x, y, 0x07);
        }
    }

    // 打印欢迎信息
    print_string("Welcome to PicoKernel!", 10, 10, 0x0f);
    print_string("This is a simple operating system kernel.", 10, 12, 0x0f);
    print_string("Kernel is running in 32-bit protected mode.", 10, 14, 0x0f);

    // 无限循环
    while (1) {
        // 什么都不做，只是保持内核运行
    }
}

