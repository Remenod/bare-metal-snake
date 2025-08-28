#pragma once

#include <lib/types.h>

#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_LEFT 3
#define KEY_RIGHT 4

#define KEY_ESC 27

void keyboard_install(void);
char get_keyboard_char(void);
int read_number(void);
uint32_t read_hex(void);
