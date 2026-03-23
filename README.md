# PicoKernel

一个简单的操作系统内核项目，用于学习操作系统原理。

## 测试提交

这是一个测试提交，用于验证GitHub Actions自动构建功能。

### 测试内存管理功能

添加了基本的内存管理功能，包括内存分配、释放和内存块管理。

### 测试GitHub Actions构建

再次测试GitHub Actions构建功能，验证计费问题是否已解决。

## 项目结构

```
PicoKernel/
├── bootloader/       # 引导加载程序
│   └── bootloader.asm  # x86引导加载程序
├── kernel/           # 内核代码
│   ├── kernel.c        # 内核主代码
│   └── linker.ld       # 链接脚本
├── build/            # 构建输出目录
├── tools/            # 工具目录
├── build.bat         # Windows构建脚本
└── README.md         # 项目说明
```

## 功能特性

- 实模式到保护模式的切换
- 基本的屏幕输出功能
- 简单的内核主循环

## 构建环境

### 必需工具

- **NASM**：用于编译引导加载程序
- **GCC**：用于编译内核（需要支持32位编译）
- **dd**：用于写入磁盘镜像（可选）
- **QEMU**：用于测试内核

### 安装建议

- **Windows**：安装 MinGW 或 Cygwin 以获取 GCC
- **Linux**：使用系统包管理器安装 gcc, nasm 和 qemu

## 构建步骤

1. **克隆项目**
   ```bash
   git clone <repository-url>
   cd PicoKernel
   ```

2. **运行构建脚本**
   - Windows: 双击 `build.bat` 或在命令行中运行
   - Linux: 使用类似的构建命令（需要修改脚本）

3. **构建输出**
   - `build/bootloader.bin`：编译后的引导加载程序
   - `build/kernel.bin`：编译后的内核
   - `build/picokernel.img`：磁盘镜像（如果dd可用）

## 测试方法

使用 QEMU 运行内核：

```bash
qemu-system-i386 -fda build/picokernel.img
```

或者使用其他x86模拟器。

## 项目目标

- 学习操作系统引导过程
- 理解实模式到保护模式的切换
- 掌握基本的内核开发技巧
- 为后续功能扩展打下基础

## 后续计划

- 添加内存管理
- 实现进程调度
- 添加文件系统支持
- 实现系统调用
- 添加驱动程序支持

## 参考资料

- [OSDev Wiki](https://wiki.osdev.org)
- [Bran's Kernel Development Tutorial](http://www.osdever.net/bkerndev)
- [JamesM's Kernel Development Tutorial](https://web.archive.org/web/20160412174753/http://www.jamesmolloy.co.uk/tutorial_html/)

## 许可证

MIT License
