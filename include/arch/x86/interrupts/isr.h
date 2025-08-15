#pragma once

#include <lib/types.h>

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC1_DATA 0x21
#define PIC_EOI 0x20

#define IDT_ENTRIES 256

typedef void (*isr_t)(void);

void isr_common_handler(uint32_t int_no);

void register_interrupt_handler(uint32_t int_no, isr_t handler);
