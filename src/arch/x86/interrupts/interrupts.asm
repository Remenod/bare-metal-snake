[BITS 32]
[EXTERN isr_common_handler]

%macro ISR_STUB 1
global isr%1
isr%1:
    pusha
    push dword %1
    call isr_common_handler
    add esp, 4
    popa
    iretd
%endmacro

%assign i 32
%rep 16
    ISR_STUB i
    %assign i i+1
%endrep
