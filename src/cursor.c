#include "cursor.h"

/*  
//  VGA CRT Controller (CRTC)

Port 0x3D4 : CRTC Address Register  
    can select vga controller register for read/write operation
    first need to define specific index, in second part data can be obtain with 0x3D5 port

Port 0x3D5 : CRTC Data Register  
    can read or write value on register selected with 0x3D4
*/

void move_cursor(const int x, const int y)
{
	uint16_t pos = y * VGA_WIDTH + x; // get pos from (x,y)

	outb(PORT_CRTC_ADDRESS, CURSOR_LOCATION_LOW); // set CRTC index to the cursor pos low byte
	outb(PORT_CRTC_DATA, (uint8_t) (pos & 0xFF)); // send lower 8 bits of the cursor pos

	outb(PORT_CRTC_ADDRESS, CURSOR_LOCATION_HIGH); // set CRTC index to the cursor pos high byte
	outb(PORT_CRTC_DATA, (uint8_t) ((pos >> 8) & 0xFF)); // send the higher 8 bits of the cursor pos
}

uint16_t get_cursor_pos(void)
{
	uint16_t pos = 0;

	outb(PORT_CRTC_ADDRESS, CURSOR_LOCATION_LOW); // set CRTC index to the cursor pos low byte
	pos |= inb(PORT_CRTC_DATA); // get 8 low bits of cursor pos

	outb(PORT_CRTC_ADDRESS, CURSOR_LOCATION_HIGH); // set CRTC index to the cursor pos high byte
	pos |= ((uint16_t)inb(PORT_CRTC_DATA)) << 8; // get 8 high bits of cursor pos
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
