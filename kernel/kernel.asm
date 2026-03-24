; 内核汇编代码
; 编译: nasm -f elf32 kernel.asm -o kernel.o

bits 32

section .text

; 导出符号
global _start
extern memory_init
extern malloc
extern free
extern memory_status

extern process_init
extern process_create
extern schedule

extern syscall_init
extern sys_getpid

extern driver_init
extern keyboard_available
extern keyboard_getch
extern desktop_draw
extern desktop_handle_input

; 内核入口点
_start:
    ; 调用内核主函数
    call kernel_main
    
    ; 防止函数返回
    jmp $  ; 无限循环

; 屏幕输出函数
print_char:
    ; 参数: char c (al), int x (bx), int y (cx), char color (dl)
    push eax
    push ebx
    push ecx
    push edx
    push esi
    
    mov esi, 0xb8000  ; 视频内存地址
    mov eax, ecx      ; y
    mov ecx, 80
    mul ecx           ; y * 80
    add eax, ebx      ; + x
    mov ebx, 2
    mul ebx           ; * 2 (每个字符占2字节)
    add esi, eax      ; 计算偏移量
    
    mov [esi], al     ; 写入字符
    mov [esi+1], dl   ; 写入颜色
    
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
    ret

print_string:
    ; 参数: const char *str (esi), int x (bx), int y (cx), char color (dl)
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi
    
    mov edi, esi  ; 保存字符串地址
.loop:
    mov al, [edi]
    cmp al, 0
    je .done
    
    ; 调用print_char
    push edx
    push ecx
    push ebx
    push eax
    call print_char
    pop eax
    pop ebx
    pop ecx
    pop edx
    
    inc edi        ; 下一个字符
    inc ebx        ; 下一列
    cmp ebx, 80
    jl .loop
    mov ebx, 0     ; 换行
    inc ecx
    jmp .loop
.done:
    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
    ret

; 内核主函数
kernel_main:
    ; 清屏
    pushad
    mov bx, 0      ; x
    mov cx, 0      ; y
    mov dl, 0x07   ; 颜色
.clear_loop:
    mov al, ' '    ; 空格
    call print_char
    inc bx
    cmp bx, 80
    jl .clear_loop
    mov bx, 0
    inc cx
    cmp cx, 25
    jl .clear_loop
    popad
    
    ; 打印欢迎信息
    pushad
    mov esi, msg_welcome
    mov bx, 10     ; x
    mov cx, 10     ; y
    mov dl, 0x0f   ; 颜色
    call print_string
    
    mov esi, msg_info1
    mov bx, 10
    mov cx, 12
    mov dl, 0x0f
    call print_string
    
    mov esi, msg_info2
    mov bx, 10
    mov cx, 14
    mov dl, 0x0f
    call print_string
    popad
    
    ; 初始化内存管理
    pushad
    mov eax, 0x1000000  ; 假设16MB内存
    call memory_init
    popad
    
    ; 测试内存分配
    pushad
    mov eax, 1024        ; 分配1KB内存
    call malloc
    mov [test_ptr], eax  ; 保存分配的内存指针
    
    ; 打印内存分配信息
    mov esi, msg_mem_alloc
    mov bx, 10
    mov cx, 16
    mov dl, 0x0f
    call print_string
    popad
    
    ; 初始化进程管理
    pushad
    call process_init
    popad
    
    ; 创建测试进程
    pushad
    mov eax, test_process
    push test_process_name  ; 进程名称
    push eax            ; 进程入口点
    push 10             ; 优先级
    call process_create
    add esp, 12         ; 清理栈
    popad
    
    ; 打印进程创建信息
    mov esi, msg_process_create
    mov bx, 10
    mov cx, 18
    mov dl, 0x0f
    call print_string
    
    ; 初始化系统调用
    pushad
    call syscall_init
    popad
    
    ; 测试系统调用
    pushad
    call sys_getpid
    ; 这里可以添加打印PID的代码
    popad
    
    ; 打印系统调用初始化信息
    mov esi, msg_syscall_init
    mov bx, 10
    mov cx, 20
    mov dl, 0x0f
    call print_string
    
    ; 初始化驱动程序
    pushad
    call driver_init
    popad
    
    ; 桌面环境主循环
.desktop_loop:
    ; 检查是否有键盘输入
    call keyboard_available
    cmp eax, 0
    je .no_input
    
    ; 获取键盘输入
    call keyboard_getch
    cmp al, 0
    je .no_input
    
    ; 处理输入
    push eax
    call desktop_handle_input
    add esp, 4
    
.no_input:
    ; 绘制桌面
    call desktop_draw
    
    ; 短暂延迟
    mov ecx, 100000
.delay:
    loop .delay
    
    jmp .desktop_loop

; 测试进程
 test_process:
    ; 打印进程运行信息
    pushad
    mov esi, msg_process_running
    mov bx, 10
    mov cx, 20
    mov dl, 0x0f
    call print_string
    popad
    
    ; 无限循环
    jmp $  ; 无限循环

section .data
msg_welcome db 'Welcome to PicoKernel!', 0
msg_info1   db 'This is a simple operating system kernel.', 0
msg_info2   db 'Kernel is running in 32-bit protected mode.', 0
msg_mem_alloc db 'Memory allocation test completed.', 0
msg_process_create db 'Process management initialized and test process created.', 0
msg_process_running db 'Test process is running!', 0
msg_syscall_init db 'System call initialized successfully.', 0
test_process_name db 'test_process', 0

test_ptr dd 0  ; 测试内存指针