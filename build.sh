#!/bin/bash

# 构建脚本（Linux版本）
# 编译引导加载程序和内核

echo "==============================="
echo "PicoKernel Build Script"
echo "==============================="

# 检查是否安装了必要的工具
if ! command -v nasm &> /dev/null; then
    echo "Error: NASM not found. Please install NASM."
    exit 1
fi

if ! command -v gcc &> /dev/null; then
    echo "Error: GCC not found. Please install GCC."
    exit 1
fi

# 创建构建目录
if [ ! -d "build" ]; then
    mkdir -p build
fi

# 编译引导加载程序
echo "Compiling bootloader..."
nasm -f bin bootloader/bootloader.asm -o build/bootloader.bin
if [ $? -ne 0 ]; then
    echo "Error: Failed to compile bootloader."
    exit 1
fi
echo "Bootloader compiled successfully."

# 编译内存管理模块
echo "Compiling memory management..."
gcc -m32 -nostdlib -nostartfiles -ffreestanding -fno-pic -c kernel/memory.c -o build/memory.o
if [ $? -ne 0 ]; then
    echo "Error: Failed to compile memory management."
    exit 1
fi
echo "Memory management compiled successfully."

# 编译进程管理模块
echo "Compiling process management..."
gcc -m32 -nostdlib -nostartfiles -ffreestanding -fno-pic -c kernel/process.c -o build/process.o
if [ $? -ne 0 ]; then
    echo "Error: Failed to compile process management."
    exit 1
fi
echo "Process management compiled successfully."

# 编译进程切换汇编
echo "Compiling process switch asm..."
nasm -f elf32 kernel/switch.asm -o build/switch.o
if [ $? -ne 0 ]; then
    echo "Error: Failed to compile process switch asm."
    exit 1
fi
echo "Process switch asm compiled successfully."

# 编译系统调用模块
echo "Compiling system calls..."
gcc -m32 -nostdlib -nostartfiles -ffreestanding -fno-pic -c kernel/syscall.c -o build/syscall.o
if [ $? -ne 0 ]; then
    echo "Error: Failed to compile system calls."
    exit 1
fi
echo "System calls compiled successfully."

# 编译内核
echo "Compiling kernel..."
nasm -f elf32 kernel/kernel.asm -o build/kernel.o
if [ $? -ne 0 ]; then
    echo "Error: Failed to compile kernel."
    exit 1
fi
echo "Kernel compiled successfully."

# 链接内核并生成二进制文件
echo "Linking kernel..."
gcc -m32 -nostdlib -nostartfiles -T kernel/linker.ld build/kernel.o build/memory.o build/process.o build/switch.o build/syscall.o -o build/kernel.bin
if [ $? -ne 0 ]; then
    echo "Error: Failed to link kernel."
    exit 1
fi
echo "Kernel linked successfully."

# 创建磁盘镜像
echo "Creating disk image..."
if [ -f "build/picokernel.img" ]; then
    rm build/picokernel.img
fi

# 创建空白镜像（1.44MB软盘）
dd if=/dev/zero of=build/picokernel.img bs=1474560 count=1
if [ $? -ne 0 ]; then
    echo "Error: Failed to create disk image."
    exit 1
fi

# 写入引导加载程序
echo "Writing bootloader to disk image..."
dd if=build/bootloader.bin of=build/picokernel.img bs=512 count=1 conv=notrunc
if [ $? -ne 0 ]; then
    echo "Error: Failed to write bootloader."
    exit 1
fi

# 写入内核
echo "Writing kernel to disk image..."
dd if=build/kernel.bin of=build/picokernel.img bs=512 seek=1 conv=notrunc
if [ $? -ne 0 ]; then
    echo "Error: Failed to write kernel."
    exit 1
fi
echo "Disk image created successfully."

echo "==============================="
echo "Build completed!"
echo "==============================="
echo "Bootloader: build/bootloader.bin"
echo "Kernel: build/kernel.bin"
echo "Disk image: build/picokernel.img"
echo ""
echo "To test the kernel, use QEMU or another emulator:"
echo "qemu-system-i386 -fda build/picokernel.img"
echo ""
