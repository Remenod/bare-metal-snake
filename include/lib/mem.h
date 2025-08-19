#pragma once

void *memcpy(void *dest, const void *src, unsigned int n);

void *memset(void *dst, int value, unsigned count);

void pokeb(unsigned seg, unsigned off, uint8_t val);

uint8_t peekb(unsigned seg, unsigned off);

void pokew(unsigned seg, unsigned off, unsigned short val);

uint16_t peekw(void *addr);