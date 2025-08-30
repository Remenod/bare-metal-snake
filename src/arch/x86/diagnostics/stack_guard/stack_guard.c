#include <kernel/diagnostics/stack_guard/stack_guard.h>

#include <timer/pit.h>
#include <kernel/diagnostics/rsod_routine.h>
#include <kernel/diagnostics/warning_routine.h>

extern void stack_guard_warning(void);
extern void stack_guard_panic(void);

void stack_guard_warning_routine(void)
{
    pop_pit_task();
    register_pit_task(stack_guard_panic);

    kernel_warning("Stack is about to overflow! Remaining capacity 25%", 10000, true);
}

void cancel_panic_ready_routine(void)
{
    pop_pit_task();
    register_pit_task(stack_guard_warning);
}

void stack_guard_install(void)
{
    register_pit_task(stack_guard_warning);
}