#pragma once

#include <lib/types.h>

#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

void remap_pic();

void pic_unmask_irq(uint8_t irq);
void pic_mask_irq(uint8_t irq);