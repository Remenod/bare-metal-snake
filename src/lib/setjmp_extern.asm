; jmp_buf_t layout (24 bytes):
; 0:  ebp
; 4:  ebx
; 8:  esi
; 12: edi
; 16: esp
; 20: eip

global setjmp
global longjmp

setjmp:
    mov eax, [esp+4]      ; buf pointer
    mov [eax],    ebp
    mov [eax+4],  ebx
    mov [eax+8],  esi
    mov [eax+12], edi
    mov [eax+16], esp
    mov ebx, [esp]      ; eip
    mov [eax+20], ebx   ; eip
    mov ebx, [eax+4]
    xor eax, eax          ; return 0
    ret

longjmp:
    mov eax, [esp+4]      ; buf pointer
    mov edx, [esp+8]      ; arg (value to return from setjmp)

    mov ebp, [eax]
    mov ebx, [eax+4]
    mov esi, [eax+8]
    mov edi, [eax+12]
    mov esp, [eax+16]

    mov ecx, [eax+20]     ; load saved eip
    mov eax, edx

    jmp ecx
