global set_cr_paging_bit

set_cr_paging_bit:
    ; enable paging (CR0.PG = bit 31)
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ; enable global pages (CR4.PGE = bit 7)
    mov eax, cr4
    or eax, 1 << 7
    mov cr4, eax

    ret
