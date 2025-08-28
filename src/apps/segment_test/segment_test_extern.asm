%macro READ_ADDR_VIA_SEG 1
global read_addr_via_%1
read_addr_via_%1:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8] 
    movzx eax, byte [%1:eax]

    pop ebp
    ret
%endmacro

READ_ADDR_VIA_SEG cs
READ_ADDR_VIA_SEG ds
READ_ADDR_VIA_SEG ss
READ_ADDR_VIA_SEG es
READ_ADDR_VIA_SEG fs
READ_ADDR_VIA_SEG gs