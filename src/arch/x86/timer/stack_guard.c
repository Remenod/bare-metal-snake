#include <timer/stack_guard.h>

#include <timer/pit.h>
#include <interrupts/rsod_routine.h>

extern void stack_guard(void);

void stack_guard_install(void)
{
    register_pit_task(stack_guard);
}