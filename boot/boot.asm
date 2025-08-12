[org 0x7c00]
KERNEL_OFFSET equ 0x1000

[bits 16]

start:
  cli
  xor ax, ax
  mov ds, ax
  mov es, ax

  ;mov ax, 0x0013     ; 13h — 320x200x256
  ;int 0x10

  ;mov ax, 0x0003     ; 03h
  ;int 0x10

  mov [BOOT_DRIVE], dl
  mov bp, 0x9000
  mov sp, bp

  mov si, msg_loading_kernel
  call print_str

  call load_kernel

  mov si, msg_kernel_loaded
  call print_str

  call switch_to_pm
  jmp $

disk_load:
  pusha
  push dx
  mov ah, 0x02
  mov al, dh
  mov ch, 0
  mov dh, 0
  mov cl, 2
  int 0x13
  pop dx
  jc disk_error
  cmp al, dh
  jne disk_error
  popa
  ret

disk_error:
  mov si, msg_disk_error
  call print_str
  jmp $

load_kernel:
  mov bx, KERNEL_OFFSET
  mov dh, 19                       ; kernel sectors count
  mov dl, [BOOT_DRIVE]
  call disk_load
  ret

; ===== PRINT TO SCREEN =====
print_str:
  pusha
  mov ah, 0x0E
.print_char:
  lodsb
  cmp al, 0
  je .done
  int 0x10
  jmp .print_char
.done:
  popa
  ret

; ===== GDT =====
gdt_start:
  dq 0x0

gdt_code:
  dw 0xffff
  dw 0x0
  db 0x0
  db 10011010b
  db 11001111b
  db 0x0

gdt_data:
  dw 0xffff
  dw 0x0
  db 0x0
  db 10010010b
  db 11001111b
  db 0x0

gdt_end:

gdt_descriptor:
  dw gdt_end - gdt_start - 1
  dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

switch_to_pm:
  cli
  lgdt [gdt_descriptor]
  mov eax, cr0
  or eax, 0x1
  mov cr0, eax
  jmp CODE_SEG:init_pm

[bits 32]

init_pm:
  mov ax, DATA_SEG
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ebp, 0x90000
  mov esp, ebp
  call KERNEL_OFFSET
  jmp $

; ===== STRINGS =====
msg_loading_kernel db "Loading kernel...", 0
msg_kernel_loaded  db "Kernel loaded!", 0
msg_disk_error     db "Disk read error", 0

BOOT_DRIVE db 0

times 446 - ($ - $$) db 0

; ===== Partition Table (64 bytes) =====
; One active partition starting at LBA=1, size — 100 sectors

db 0x80                    ; status — active
db 0x01, 0x01, 0x00        ; CHS start (doesn't matter, set to minimum)
db 0x06                    ; partition type (FAT16, or any valid one)
db 0xFE, 0xFF, 0xFF        ; CHS end (maximum — for reliability)
dd 0x00000001              ; LBA start = 1 (after MBR)
dd 0x00000064              ; size = 100 sectors (can be changed)

times 16 * 3 db 0          ; the remaining 3 records are zero

; ===== Boot Signature =====
dw 0xAA55
