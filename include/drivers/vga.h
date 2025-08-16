#pragma once

#include <stdint.h>

#define FONT_HEIGHT 16

void demo_graphics();
void set_text_m();
void read_regs(unsigned char *regs);
void write_regs(unsigned char *regs);
void set_plane(int plane);
void read_palette(uint8_t palette[256][3]);
void write_palette(uint8_t palette[256][3]);
void write_font(uint8_t font[256][FONT_HEIGHT]);
void read_font(uint8_t font[256][FONT_HEIGHT]);
