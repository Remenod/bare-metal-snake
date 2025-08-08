#pragma once

#include <stdint.h>

#define IDT_ENTRIES 256

void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags);

void idt_install();
