#pragma once

#include <lib/types.h>

uint32_t strlen(const char *str);

char *int_to_str(int32_t value, char *str);

char *uint_to_str(uint32_t value, char *str);

char *uint_to_str_hex(uint32_t value, char *str);

uint32_t strcpy(char *dst, const char *src);

void strcat(char *dst, const char *src);