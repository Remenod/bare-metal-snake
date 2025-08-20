#pragma once

#include <lib/types.h>

#define BLACK 0x0
#define BLUE 0x1
#define GREEN 0x2
#define CYAN 0x3
#define RED 0x4
#define MAGENTA 0x5
#define BROWN 0x6
#define LIGHT_GREY 0x7
#define DARK_GREY 0x8
#define LIGHT_BLUE 0x9
#define LIGHT_GREEN 0xA
#define LIGHT_CYAN 0xB
#define LIGHT_RED 0xC
#define LIGHT_MAGENTA 0xD
#define YELLOW 0xE
#define WHITE 0xF

void put_char(uint16_t pos, unsigned char c);

void put_string(uint16_t start_pos, const char text[]);

void clear_screen();

void fill_screen(unsigned char symb, uint8_t fg_color, uint8_t bg_color);

void set_cursor_visibility(bool_t visible);

void move_cursor(uint16_t pos);

void print(const char *text);

void print_dec(const int num);

void print_char(char c);
