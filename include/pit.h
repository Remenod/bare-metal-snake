#pragma once

#include <pic.h>
#include <stdint.h>

#define PIT_FREQUENCY 1193182 // base frequency of PIT in Hz
#define IRQ_TIMER 0           // IRQ 0 for PIT
#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40

void pit_init(uint32_t frequency);

uint64_t get_timer_ticks();

void sleep(uint32_t ms);