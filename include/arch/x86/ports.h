#pragma once

#include <lib/types.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);

inline uint16_t inw(uint16_t port);
inline void outw(uint16_t port, uint16_t data);

inline uint32_t inl(uint16_t port);
inline void outl(uint16_t port, uint32_t data);