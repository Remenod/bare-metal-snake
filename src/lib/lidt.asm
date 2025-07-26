[BITS 32]
[GLOBAL lidt]

lidt:
    mov eax, [esp + 4]
    lidt [eax]
    ret
