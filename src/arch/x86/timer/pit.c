#include <timer/pit.h>

#include <ports.h>
#include <interrupts/isr.h>
#include <interrupts/pic.h>

static volatile uint64_t timer_ticks = 0;

static volatile uint32_t timer_frequency;

void pit_handler()
{
    timer_ticks++;
    outb(PIC1_COMMAND, PIC_EOI);
}

void pit_init(uint32_t frequency)
{
    timer_frequency = frequency;
    uint16_t divisor = (uint16_t)(PIT_FREQUENCY / frequency);

    // Programming PIT: mode 2 (rate generator), access to both bytes, channel 0
    outb(PIT_COMMAND_PORT, 0x36);

    // Send the least significant byte of the divisor
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);

    // Send the most significant byte of the divisor
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);

    register_interrupt_handler(IRQ_TIMER + 32, pit_handler);
    pic_unmask_irq(IRQ_TIMER);
}

uint64_t get_timer_ticks()
{
    return timer_ticks;
}

uint32_t get_timer_frequency()
{
    return timer_frequency;
}

void sleep(uint32_t ms)
{
    uint64_t target = get_timer_ticks() + ms;
    while (get_timer_ticks() < target)
        asm volatile("hlt");
}
