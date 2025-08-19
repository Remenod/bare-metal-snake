#pragma once

#include <lib/types.h>

void put_char(uint16_t pos, unsigned char c);

void put_string(uint16_t start_pos, const char text[]);

void clear_screen();

void set_cursor_visibility(bool_t visible);

void move_cursor(uint16_t pos);

void print(const char *text);

void print_dec(const int num);

void print_char(char c);
