#pragma once
#include <stdint.h>
#include <ports.h>
#include <string.h>

void put_char(int pos, char c);

void put_string(int start_pos, char text[]);

void clear_screen();

void disable_vga_cursor();

void move_cursor(uint16_t pos);
