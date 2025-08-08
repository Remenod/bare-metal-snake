#include <pit.h>

#include <ports.h>
#include <isr.h>
#include <pic.h>

static volatile uint64_t timer_ticks = 0;

void pit_handler()
{
    timer_ticks++;
    // Send EOI to interrupt controller
    outb(0x20, 0x20);
}

void pit_init(uint32_t frequency)
{
    // Calculate the divisor for the given frequency
    uint16_t divisor = (uint16_t)(PIT_FREQUENCY / frequency);

    // Programming PIT: mode 2 (rate generator), access to both bytes, channel 0
    outb(PIT_COMMAND_PORT, 0x36);

    // Send the least significant byte of the divisor
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);

    // Send the most significant byte of the divisor
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);

    register_interrupt_handler(IRQ_TIMER + 32, pit_handler);

    // Make sure that PIC allows IRQ0
    pic_unmask_irq(IRQ_TIMER);
}

uint64_t get_timer_ticks()
{
    return timer_ticks;
}

void sleep(uint32_t ms)
{
    uint64_t target = get_timer_ticks() + ms;
    while (get_timer_ticks() < target)
        asm volatile("hlt");
}
