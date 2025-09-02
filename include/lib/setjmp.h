#pragma once

#include <lib/types.h>

typedef struct
{
    size_t EBP;
    size_t EBX;
    size_t ESI;
    size_t EDI;
    size_t ESP;
    size_t EIP;
} jmp_buf_t;

int setjmp(jmp_buf_t *buf);

void longjmp(jmp_buf_t *buf, int arg);