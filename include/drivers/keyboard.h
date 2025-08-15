#pragma once

#include <stdbool.h>

#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_LEFT 3
#define KEY_RIGHT 4

#define KEY_ESC 27

void keyboard_install();
char get_char();
int read_number();
bool_t check_exit_key(void);
