; 进程切换的汇编实现

bits 32

section .text

global __process_switch_asm
extern __current_process

; 进程切换的汇编实现
; 参数: next_process (ebx)
__process_switch_asm:
    ; 保存当前进程的上下文
    ; 保存通用寄存器
    pusha
    
    ; 保存EBP
    push ebp
    
    ; 保存ESP
    mov eax, [__current_process]
    mov [eax + 8], esp  ; 保存ESP到进程控制块
    
    ; 加载下一个进程的上下文
    mov [__current_process], ebx
    
    ; 加载ESP
    mov esp, [ebx + 8]  ; 从进程控制块加载ESP
    
    ; 恢复EBP
    pop ebp
    
    ; 恢复通用寄存器
    popa
    
    ; 返回
    ret
