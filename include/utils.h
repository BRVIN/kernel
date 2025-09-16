#ifndef KFS_UTILS_H
#define KFS_UTILS_H

#include <stddef.h>
#include <stdbool.h>
#include "screen.h"
#include "io.h"

bool isprint(const char c);

int strcmp(const char *s1, const char *s2);

size_t strlen(const char *str);

void putstr(const char *str);

void putnbr_at(int n, uint8_t color, int x, int y);

void puthex_at(const uint8_t scancode, const int x, const int y);

void putcoord(const int cx, const int cy, uint8_t color, const int x, const int y);

int contains(char c, const char *str);

void print_hex(uint32_t value);

#endif /* KFS_UTILS_H */
