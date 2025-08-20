[BITS 32]
[EXTERN isr_common_handler]

; ------------------------------
; Macro for ISR without error code
; ------------------------------
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

; ------------------------------
; Macro for ISR with error code
; ------------------------------
%macro ISR_STUB_ERR 1
global isr%1
isr%1:
    pusha
    push dword %1
    push dword [esp+36]
    call isr_common_handler
    add esp, 8
    popa
    iretd
%endmacro

; ------------------------------
; ISR generation for CPU exceptions
; ------------------------------
%define EXCEPTIONS_WITH_ERRCODE 8,10,11,12,13,14,17

%assign i 0
%rep 32
    %define ERR 0
    %rep 7
        %if i = EXCEPTIONS_WITH_ERRCODE[%repcount-1]
            %define ERR 1
        %endif
    %endrep

    %if ERR
        ISR_STUB_ERR i
    %else
        ISR_STUB i
    %endif

    %assign i i+1
%endrep

; ------------------------------
; ISR generation for IRQ 0–15
; ------------------------------
%assign i 32
%rep 16
    ISR_STUB i
    %assign i i+1
%endrep
