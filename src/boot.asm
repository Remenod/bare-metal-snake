[org 0x7c00]
KERNEL_OFFSET equ 0x1000

[bits 16]

start:
  cli
  xor ax, ax
  mov ds, ax
  mov es, ax

  mov [BOOT_DRIVE], dl
  mov bp, 0x9000
  mov sp, bp

  call load_kernel
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
  jmp $

load_kernel:
  mov bx, KERNEL_OFFSET
  mov dh, 1
  mov dl, [BOOT_DRIVE]
  call disk_load
  ret

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

BOOT_DRIVE db 0

times 510 - ($ - $$) db 0
dw 0xaa55
