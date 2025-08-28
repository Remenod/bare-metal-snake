#pragma once

#include <interrupts/pic.h>
#include <lib/types.h>

#define PIT_FREQUENCY 1193182 // base frequency of PIT in Hz
#define IRQ_TIMER 0           // IRQ 0 for PIT
#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40

void pit_init(uint32_t frequency);

void register_pit_task(func_t task);

void pop_pit_task(void);

uint64_t get_timer_ticks(void);

uint32_t get_timer_frequency(void);

void sleep(uint32_t ms);