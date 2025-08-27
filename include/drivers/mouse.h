#pragma once

#include <lib/types.h>

typedef struct
{
    int8_t dx;
    int8_t dy;
    uint8_t buttons;
} mouse_packet_t;

void mouse_install(void);