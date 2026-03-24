; 引导加载程序
; 编译: nasm -f bin bootloader.asm -o bootloader.bin

org 0x7c00          ; 引导加载程序的起始地址
bits 16             ; 16位实模式

start:
    ; 初始化段寄存器
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00  ; 栈指针指向引导加载程序的起始地址

    ; 清屏
    mov ah, 0x00
    mov al, 0x03
    int 0x10

    ; 打印欢迎信息
    mov si, msg_welcome
    call print_string

    ; 读取内核到内存
    mov ah, 0x02    ; 读取扇区功能
    mov al, 0x08    ; 读取8个扇区
    mov ch, 0x00    ; 柱面0
    mov cl, 0x02    ; 扇区2 (引导扇区是扇区1)
    mov dh, 0x00    ; 磁头0
    ; dl 保持启动时的设备号，不硬编码
    mov bx, 0x1000  ; 目标地址: 0x1000:0x0000
    mov es, bx
    mov bx, 0x0000
    int 0x13        ; 调用BIOS中断

    ; 检查读取是否成功
    jc error

    ; 打印加载成功信息
    mov si, msg_loaded
    call print_string

    ; 切换到保护模式
    cli             ; 禁用中断
    lgdt [gdt_descriptor] ; 加载GDT
    mov eax, cr0
    or eax, 1       ; 设置PE位
    mov cr0, eax

    ; 远跳转到保护模式代码
    jmp dword 0x08:protected_mode

error:
    mov si, msg_error
    call print_string
    hlt

print_string:
    mov ah, 0x0e    ; BIOS teletype功能
.repeat:
    lodsb           ; 加载字符到al
    cmp al, 0       ; 检查是否结束
    je .done
    int 0x10        ; 调用BIOS中断
    jmp .repeat
.done:
    ret

; 全局描述符表
GDT:
    ; 空描述符
    dd 0
    dd 0

    ; 代码段描述符
    dw 0xffff       ; 段界限
    dw 0x0000       ; 基地址低16位
    db 0x00         ; 基地址中8位
    db 0x9a         ; 访问权限: 代码段, 可读可执行
    db 0xcf         ; 标志: 4KB粒度, 32位
    db 0x00         ; 基地址高8位

    ; 数据段描述符
    dw 0xffff       ; 段界限
    dw 0x0000       ; 基地址低16位
    db 0x00         ; 基地址中8位
    db 0x92         ; 访问权限: 数据段, 可读写
    db 0xcf         ; 标志: 4KB粒度, 32位
    db 0x00         ; 基地址高8位

gdt_descriptor:
    dw 23           ; GDT大小 - 1
    dd GDT          ; GDT地址

; 保护模式代码
bits 32
protected_mode:
    ; 初始化段寄存器
    mov ax, 0x10    ; 数据段选择子
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000  ; 栈指针，与内核中的设置保持一致

    ; 跳转到内核
    jmp 0x10000     ; 内核加载地址

; 消息
msg_welcome db 'PicoKernel Bootloader', 0x0a, 0x0d, 0
msg_loaded  db 'Kernel loaded successfully', 0x0a, 0x0d, 0
msg_error   db 'Error loading kernel', 0x0a, 0x0d, 0

; 填充到512字节并添加引导标志
times 510-($-$$) db 0
dw 0xaa55          ; 引导标志