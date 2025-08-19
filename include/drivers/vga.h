#pragma once

#include <lib/types.h>

#define FONT_HEIGHT 16

void set_graphics_mode(void);
void set_text_mode(void);
void write_font(uint8_t font[256][FONT_HEIGHT]);
