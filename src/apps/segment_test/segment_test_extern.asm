global test_gs_addres_acces
test_gs_addres_acces:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]       ; отримати аргумент addr
    movzx eax, byte [gs:eax] ; прочитати байт через GS і розширити до 32-біт

    pop ebp
    ret                       ; повертає значення в EAX
