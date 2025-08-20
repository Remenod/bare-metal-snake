#pragma once

#include <lib/types.h>

struct cpu_state
{
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags;
    uint32_t useresp;
    uint32_t ss;
    uint32_t eax, ebx, ecx, edx, esi, edi, ebp;
};

void show_rsod(const char *msg, struct cpu_state *state);

void rsod_add_log(const char *msg);