#ifndef KFS_SCREEN_H
#define KFS_SCREEN_H

#include <stddef.h>
#include <stdint.h>

#include "io.h"
#include "utils.h"
#include "draw.h"
#include "cursor.h"

#define EMPTY_CHAR ' '
#define EMPTY_COLOR 4
#define EMPTY_VGA ((uint16_t)EMPTY_CHAR | ((uint16_t)EMPTY_COLOR << 8))

extern const size_t VGA_WIDTH;  // 80;
extern const size_t VGA_HEIGHT; // 25;

enum vga_color
{
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_YELLOW = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_YELLOW = 14,
  VGA_COLOR_WHITE = 15,
};

extern size_t g_y[3];
extern size_t g_x[3];
extern uint8_t g_color[3];
extern uint16_t *g_buffer;
extern uint16_t g_screens[3][2000];
extern uint8_t g_current_screen;

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);

/* INIT */
void remove_logo(void);
void init_screen(void);

/* COLORS */
void screen_green(void);
void screen_yellow(void);
void screen_magenta(void);
void screen_color(enum vga_color fg, enum vga_color bg);
void recolor_screen_at(uint16_t start_pos);

/* PUTS */
void putchar(const char c);
void putchar_at(const char c, uint8_t color, size_t x, size_t y);
void putstr_at(const char *str, uint8_t color, size_t ox, size_t y);

void newline(void);

void putchar(const char c);

#endif /* KFS_SCREEN_H */
