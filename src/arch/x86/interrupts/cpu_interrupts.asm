[BITS 32]
[EXTERN isr_exception_handler]
%macro ISR_EXC_COMMON 1
global isr%1
isr%1:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    %ifidn %1, 8 || %1 >= 10 && %1 <= 14 || %1 == 17
        
    %else
        push dword 0
    %endif

    push dword %1        ; isr_no
    call isr_exception_handler
    add esp, 8           ; free stack

    pop gs
    pop fs
    pop es
    pop ds
    popa
    iretd
%endmacro

%assign i 0
%rep 32
    ISR_EXC_COMMON i
    %assign i i+1
%endrep

