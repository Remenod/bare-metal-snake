#pragma once

#include <lib/types.h>

void kernel_warning(const char *msg, uint32_t duration, bool_t manage_irq);
void calibrate_warning_iter_per_tick(void);