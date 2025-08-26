global read_arrd_via_gs
read_arrd_via_gs:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8] 
    movzx eax, byte [gs:eax]

    pop ebp
    ret
