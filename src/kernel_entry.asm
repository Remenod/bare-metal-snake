[BITS 32]
[GLOBAL _start]
extern kernel_main

_start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    mov edi, 0xB8000
    mov ecx, 80 * 25

    mov eax, 0x07200720
clear_loop:
    stosd
    loop clear_loop

    call kernel_main

.loop:
    jmp .loop
