#include "show.h"

void show_guides()
{
	const uint8_t color = VGA_COLOR_WHITE;

	for (size_t y = 0; y < VGA_HEIGHT; y++)
		putstr_at("|", color, VGA_WIDTH / 2, y);

	for (size_t x = 0; x < VGA_WIDTH; x++)
		putstr_at("_", color, x, VGA_HEIGHT / 2);

	putstr_at("x", color, VGA_WIDTH / 2, VGA_HEIGHT / 2);
}

void show_42()
{
	const uint8_t color = VGA_COLOR_WHITE;
	int y = 10;
	int x = 25;
	putstr_at("          :::      ::::::::  ", color, x, y++);
	putstr_at("        :+:      :+:    :+:  ", color, x, y++);
	putstr_at("      +:+ +:+         +:+    ", color, x, y++);
	putstr_at("    +#+  +:+       +#+       ", color, x, y++);
	putstr_at("  +#+#+#+#+#+   +#+          ", color, x, y++);
	putstr_at("       #+#    #+#            ", color, x, y++);
	putstr_at("      ###   #####es#         ", color, x, y++);
}

void show_tabs()
{
	const uint8_t color = VGA_COLOR_WHITE;

	for (size_t x = 0; x < VGA_WIDTH; x++)
	{
		putstr_at("-", color, x, 0);
		putstr_at("-", color, x, 2);
	}
	for (size_t x = 0; x < 46; x++)
	{
		putstr_at(" ", color, x, 1);
	}

	int x = 36;
	putstr_at("#", VGA_COLOR_GREEN, x + 1, 1);
	putstr_at("#", VGA_COLOR_YELLOW, x + 5, 1);
	putstr_at("#", VGA_COLOR_MAGENTA, x + 9, 1);
	if (g_color[g_current_screen] == vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK))
	{
		putstr_at("[", VGA_COLOR_WHITE, x, 1);
		putstr_at("]", VGA_COLOR_WHITE, x + 2, 1);
	}
	if (g_color[g_current_screen] == vga_entry_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK))
	{
		putstr_at("[", VGA_COLOR_WHITE, x + 4, 1);
		putstr_at("]", VGA_COLOR_WHITE, x + 6, 1);
	}
	if (g_color[g_current_screen] == vga_entry_color(VGA_COLOR_MAGENTA, VGA_COLOR_BLACK))
	{
		putstr_at("[", VGA_COLOR_WHITE, x + 8, 1);
		putstr_at("]", VGA_COLOR_WHITE, x + 10, 1);
	}

	if (g_current_screen == 0)
	{
		putstr_at(" [ TAB 1 ]  ", VGA_COLOR_WHITE, 0, 1);
		putstr_at("   TAB 2    ", VGA_COLOR_WHITE, 12, 1);
		putstr_at("   TAB 3    ", VGA_COLOR_WHITE, 24, 1);
	}
	if (g_current_screen == 1)
	{
		putstr_at("   TAB 1    ", VGA_COLOR_WHITE, 0, 1);
		putstr_at(" [ TAB 2 ]  ", VGA_COLOR_WHITE, 12, 1);
		putstr_at("   TAB 3    ", VGA_COLOR_WHITE, 24, 1);
	}
	if (g_current_screen == 2)
	{
		putstr_at("   TAB 1    ", VGA_COLOR_WHITE, 0, 1);
		putstr_at("   TAB 2    ", VGA_COLOR_WHITE, 12, 1);
		putstr_at(" [ TAB 3 ]  ", VGA_COLOR_WHITE, 24, 1);
	}
}

void draw_dbg_scancode(const uint8_t scancode)
{
	for (size_t x = 0; x < VGA_WIDTH - 1; x++)
	{
		putstr_at(" ", VGA_COLOR_WHITE, x, 1);
	}
	const uint8_t color = VGA_COLOR_WHITE;
	putstr_at("[", color, 76, 1);
	puthex_at(scancode, 77, 1);
	putstr_at("]", color, 79, 1);
}

void draw_dbg_cursor_pos(void)
{
	const uint8_t color = VGA_COLOR_WHITE;
	// global var cursor position
	int cx = g_x[g_current_screen];
	int cy = g_y[g_current_screen];
	putcoord(cx, cy, color, 60, 1);

	// real cursor position
	uint16_t pos = get_cursor_pos();
	cx = get_cursor_x(pos);
	cy = get_cursor_y(pos);
	putcoord(cx, cy, color, 68, 1);
}