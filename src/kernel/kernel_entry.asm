[BITS 32]
[GLOBAL _start]
extern kernel_main

_start:
    call kernel_main

.loop:
    jmp .loop
