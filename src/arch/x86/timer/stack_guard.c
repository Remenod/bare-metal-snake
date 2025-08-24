#include <timer/pit.h>

void stack_guard()
{
}

void stack_guard_install(void)
{
    register_pit_task(stack_guard);
}