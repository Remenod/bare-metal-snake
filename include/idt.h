#pragma once

#include <stdint.h>
#include <stdint.h>
#include <ports.h>
#include <pic.h>

#define IDT_ENTRIES 256

void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags);

void idt_install();
