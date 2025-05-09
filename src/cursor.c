#include "cursor.h"

// TODO define cursor ports adress

//  VGA CRT Controller (CRTC)
// jtiojiojio

void move_cursor(const int x, const int y)
{
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t get_cursor_pos(void)
{
	uint16_t pos = 0;
	outb(0x3D4, 0x0F);
	pos |= inb(0x3D5);
	outb(0x3D4, 0x0E);
	pos |= ((uint16_t)inb(0x3D5)) << 8;
	return pos;
}

uint16_t get_cursor_x(const uint16_t pos)
{
	return pos % VGA_WIDTH;
}

uint16_t get_cursor_y(const uint16_t pos)
{
	return pos / VGA_WIDTH;
}
