#pragma once

#include <lib/types.h>
#include <interrupts/isr.h>

_Noreturn void show_rsod(const char *msg, const cpu_state_t *state, uint32_t int_no);

void rsod_add_log(const char *msg);