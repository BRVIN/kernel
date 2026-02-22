#ifndef KFS_UTILS_H
#define KFS_UTILS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "screen.h"
#include "io.h"

bool isprint(const char c);
int strcmp(const char *s1, const char *s2);
size_t strlen(const char *str);

/* PUTSTR */
void putstr(const char *str);
void putstr_nl(const char *str);

/* PUTNBR */
void putnbr(int n);
void putnbr_nl(int n);
void putnbr_at(int n, uint8_t color, int x, int y);

/* PUTHEX */
void puthex(uint32_t val);
void puthex_nl(uint32_t val);
void puthex_t(const char *str, uint32_t val);
void puthex_tnl(const char *str, uint32_t val);
void puthex_at(const uint8_t scancode, const int x, const int y);

/* PUTCOORD */
void putcoord(const int cx, const int cy, uint8_t color, const int x, const int y);

/* GDT */
void print_gdt_selectors();
void print_gdt_segments();

#endif /* KFS_UTILS_H */
