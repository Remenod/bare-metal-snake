[BITS 16]
[ORG 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax

    ; Loab 1 kernel sector to 0x1000
    mov ah, 0x02
    mov al, 1   ; sectors count
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x00
    mov bx, 0x1000
    int 0x13
    jc disk_error

    ; Set GDT
    lgdt [gdt_descriptor]

    ; Enable protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; far jump to 32-bit code
    jmp 08h:protected_mode_entry

disk_error:
    hlt
    jmp disk_error

; --------------------------
; GDT for protected mode
; --------------------------
gdt_start:
    dq 0x0000000000000000     ; null
    dq 0x00CF9A000000FFFF     ; code segment: base=0, limit=4GB, exec/read
    dq 0x00CF92000000FFFF     ; data segment: base=0, limit=4GB, read/write
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; --------------------------
; 32-bit protected mode
; --------------------------
[BITS 32]
protected_mode_entry:
    mov ax, 0x10         ; data selector (2-й desc → 0x10)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    call dword 0x1000    ; kernel call

    jmp $

times 510 - ($ - $$) db 0
dw 0xAA55
