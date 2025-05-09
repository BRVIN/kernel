#include "cursor.h"

/*
//  VGA CRT Controller (CRTC)
Port 0x3D4 : CRT Controller Index Register (CRTC Address Register)
    Ce port est utilisé pour sélectionner l'index du registre CRT que l'on souhaite lire ou écrire.
    Il agit comme un sélecteur de registre parmi les registres internes du contrôleur CRT.
    Une fois l'index défini, la lecture/écriture se fait via le port 0x3D5.

Port 0x3D5 : CRT Controller Data Register (CRTC Data Register)
    Ce port est utilisé pour lire ou écrire la valeur dans le registre CRT sélectionné via 0x3D4.
    En d'autres termes, après avoir défini l'index sur le port 0x3D4, ce port permet d'accéder aux données du registre correspondant.
*/

void move_cursor(const int x, const int y)
{
	uint16_t pos = y * VGA_WIDTH + x;

	outb(PORT_CRTC_ADDRESS, CURSOR_LOCATION_LOW);
	outb(PORT_CRTC_DATA, (uint8_t) (pos & 0xFF));
	outb(PORT_CRTC_ADDRESS, CURSOR_LOCATION_HIGH);
	outb(PORT_CRTC_DATA, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t get_cursor_pos(void)
{
	uint16_t pos = 0;
	outb(PORT_CRTC_ADDRESS, CURSOR_LOCATION_LOW);
	pos |= inb(PORT_CRTC_DATA);
	outb(PORT_CRTC_ADDRESS, CURSOR_LOCATION_HIGH);
	pos |= ((uint16_t)inb(PORT_CRTC_DATA)) << 8;
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
