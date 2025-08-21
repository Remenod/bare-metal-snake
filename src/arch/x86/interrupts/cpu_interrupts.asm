[BITS 32]
[EXTERN isr_exception_handler]
[EXTERN isr_stateless_exception_handler]

%macro ISR_EXC_COMMON 1
global isr%1
isr%1:
    ; EIP CS EFLAFS already pushed by cpu

    %ifidn %1, 8 || %1 >= 10 && %1 <= 14 || %1 == 17
        ;err_code already pushed by cpu
    %else
        push dword 0        ; err_code = 0
    %endif

    push dword %1           ; interrupt number

    pusha    ; EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_exception_handler

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 20 ; clean EIP, CS, EFLAFS, int_no and err_code
    iretd
%endmacro

%macro ISR_EXC_STATELESS 1
global isr%1
isr%1:
    push dword %1           ; interrupt number

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_stateless_exception_handler

    add esp, 4
    iretd
%endmacro

%assign i 0
%rep 32
    %if i == 3
        ISR_EXC_STATELESS i
    %else
        ISR_EXC_COMMON i
    %endif
    %assign i i+1
%endrep
