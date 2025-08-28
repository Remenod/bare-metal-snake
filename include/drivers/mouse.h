#pragma once

#include <lib/types.h>

typedef struct
{
    int8_t dx;
    int8_t dy;
    uint8_t buttons;
} mouse_packet_t;

typedef bool_t (*ui_bound_func_t)(uint16_t cursor_x, uint16_t cursor_y);

typedef struct
{
    ui_bound_func_t bound;
    func_t mouse1_handler;
    func_t mouse2_handler;
    func_t mouse3_handler;

} mouse_ui_element_t;

void register_ui_element(uint8_t layer, mouse_ui_element_t ui_element);

void reset_ui_structure(void);

void reset_ui_layer(uint8_t layer);

void mouse_install(void);