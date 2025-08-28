#pragma once

#include <lib/types.h>

uint32_t strlen(const char *str);

char *int_to_str(int32_t value, char *str);

char *uint_to_str(uint32_t value, char *str);

char *uint_to_str_hex(uint32_t value, char *str);

uint32_t strcpy(char *dst, const char *src);

void strncpy(char *dst, const char *src, uint32_t n);

int8_t strcmp(const char *a, const char *b);

void strcat(char *dst, const char *src);