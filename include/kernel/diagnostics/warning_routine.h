#pragma once

#include <lib/types.h>

void kernel_warning(const char *msg, uint32_t duration, bool_t manage_irq);
void init_kernel_warning_routine(void);