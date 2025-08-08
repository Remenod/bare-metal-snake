#include "snake.h"

#include <stdbool.h>
#include <keyboard.h>

void snake_main()
{
    while (true)
    {
        if (check_exit_key())
            return;
    }
}