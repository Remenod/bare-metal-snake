#pragma once

#include <stdint.h>

void put_char(uint16_t pos, char c);

void put_string(uint16_t start_pos, char text[]);

void clear_screen();

void disable_vga_cursor();

void move_cursor(uint16_t pos);

void print(const char *text);

void print_dec(const int num);

void print_char(char c);
