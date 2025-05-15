#ifndef KFS_UTILS_H
#define KFS_UTILS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long size_t;

#include "screen.h"
#include "io.h"

int isprint(const char c);

int strcmp(const char *s1, const char *s2);

size_t strlen(const char *str);

void putstr(const char *str);

void putnbr_at(int n, uint8_t color, int x, int y);

void puthex_at(const uint8_t scancode, const int x, const int y);

void putcoord(const int cx, const int cy, uint8_t color, const int x, const int y);

int contains(char c, const char *str);

#endif /* KFS_UTILS_H */
