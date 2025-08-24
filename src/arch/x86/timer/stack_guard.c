#include <timer/pit.h>

#include <interrupts/rsod_routine.h>

extern void stack_guard(void);

void stack_overflow_routine(void)
{
    rsod_add_log("[STACK GUARD] Overflow detected!");
    rsod_add_log("ESP below guard limit (0x60000)");
    asm volatile("int $12");
}

void stack_guard_install(void)
{
    register_pit_task(stack_guard);
}