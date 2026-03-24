@echo off

REM 构建脚本
REM 编译引导加载程序和内核

echo ===============================
echo PicoKernel Build Script
echo ===============================

REM 检查是否安装了必要的工具
where nasm > nul 2>&1
if %errorlevel% neq 0 (
    echo Error: NASM not found. Please install NASM.
    pause
    exit /b 1
)

where gcc > nul 2>&1
if %errorlevel% neq 0 (
    echo Error: GCC not found. Please install MinGW or Cygwin.
    pause
    exit /b 1
)

REM 创建构建目录
if not exist build mkdir build

REM 编译引导加载程序
echo Compiling bootloader...
nasm -f bin bootloader\bootloader.asm -o build\bootloader.bin
if %errorlevel% neq 0 (
    echo Error: Failed to compile bootloader.
    pause
    exit /b 1
)
echo Bootloader compiled successfully.

REM 编译内核
echo Compiling kernel...
nasm -f elf32 kernel\kernel.asm -o build\kernel.o
if %errorlevel% neq 0 (
    echo Error: Failed to compile kernel.
    pause
    exit /b 1
)
echo Kernel compiled successfully.

REM 链接内核
echo Linking kernel...
gcc -m32 -nostdlib -nostartfiles -T kernel\linker.ld build\kernel.o -o build\kernel.elf
if %errorlevel% neq 0 (
    echo Error: Failed to link kernel.
    pause
    exit /b 1
)
echo Kernel linked successfully.

REM 转换为二进制格式
echo Converting to binary format...
objcopy -O binary build\kernel.elf build\kernel.bin
if %errorlevel% neq 0 (
    echo Error: Failed to convert kernel to binary format.
    pause
    exit /b 1
)
echo Kernel converted to binary format successfully.

REM 创建磁盘镜像
echo Creating disk image...
if exist build\picokernel.img del build\picokernel.img

REM 创建空白镜像（1.44MB软盘）
fsutil file createnew build\picokernel.img 1474560
if %errorlevel% neq 0 (
    echo Error: Failed to create disk image.
    pause
    exit /b 1
)

REM 写入引导加载程序
echo Writing bootloader to disk image...
REM 使用dd命令写入（需要安装Git for Windows或其他包含dd的工具）
REM 如果没有dd，可以使用其他方法写入

REM 使用PowerShell命令写入引导加载程序和内核
echo Writing bootloader to disk image...
powershell -Command "$bootloader = [System.IO.File]::ReadAllBytes('build\bootloader.bin'); $image = [System.IO.File]::ReadAllBytes('build\picokernel.img'); [System.Buffer]::BlockCopy($bootloader, 0, $image, 0, $bootloader.Length); [System.IO.File]::WriteAllBytes('build\picokernel.img', $image)"
if %errorlevel% neq 0 (
    echo Error: Failed to write bootloader.
    pause
    exit /b 1
)

echo Writing kernel to disk image...
powershell -Command "$kernel = [System.IO.File]::ReadAllBytes('build\kernel.bin'); $image = [System.IO.File]::ReadAllBytes('build\picokernel.img'); [System.Buffer]::BlockCopy($kernel, 0, $image, 512, $kernel.Length); [System.IO.File]::WriteAllBytes('build\picokernel.img', $image)"
if %errorlevel% neq 0 (
    echo Error: Failed to write kernel.
    pause
    exit /b 1
)
echo Disk image created successfully.

echo ===============================
echo Build completed!
echo ===============================
echo Bootloader: build\bootloader.bin
echo Kernel: build\kernel.bin
echo Disk image: build\picokernel.img (if dd was available)
echo 
echo To test the kernel, use QEMU or another emulator:
echo qemu-system-i386 -fda build\picokernel.img
echo 
pause