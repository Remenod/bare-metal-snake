#pragma once

#include <lib/types.h>
#include <interrupts/isr.h>

void show_rsod(const char *msg, const struct cpu_state *state);

void rsod_add_log(const char *msg);