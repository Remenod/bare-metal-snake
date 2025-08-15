#pragma once

#include <lib/types.h>

uint32_t strlen(const char *str);

char *int_to_str(int value, char *str);

uint32_t strcpy(char *dst, const char *src);

void strcat(char *dst, const char *src);