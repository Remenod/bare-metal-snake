global _start

extern efi_main

section .text
_start:
    mov rdi, rdi        ; ImageHandle
    mov rsi, rsi        ; SystemTable
    call efi_main
    mov eax, 0
    ret

section .note.GNU-stack
