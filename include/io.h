#ifndef KFS_IO_H
#define KFS_IO_H


#include "utils.h"

// PS/2 controller ports
// state of the PS/2 controller
#define PORT_PS2_STATUS 0x64
// keyboard press/release, mouse, ps/2 cmds (test/config)
#define PORT_PS2_DATA 0x60
// PS/2 utility functions
uint8_t is_ps2_data_ready();
uint8_t read_ps2_scancode();

/*
    What is CMOS?
    CMOS (Complementary Metal-Oxide-Semiconductor) is a small memory on the motherboard powered by a battery.
    It stores important system settings:
        Date and time.
        BIOS configuration.
        Hardware information (e.g., boot order).
    Accessing CMOS data is done via the RTC (Real-Time Clock) and I/O ports.
*/
// 0x70 (CMOS_ADDRESS): Used to select the address to read/write.
#define PORT_CMOS_ADDRESS 0x70
// 0x71 (CMOS_DATA): Used to read or write the data from the selected address.
#define PORT_CMOS_DATA 0x71
// CMOS
unsigned char read_cmos(unsigned char value);

//  VGA CRT Controller (CRTC)
#define PORT_CRTC_ADDRESS 0x3D4
#define PORT_CRTC_DATA 0x3D5
// Index 0x0F : Cursor Location Low
// Ce registre contient les 8 bits de poids faible (bits 0 à 7) de la position du curseur sur l'écran.
#define CURSOR_LOCATION_LOW 0x0F
// Index 0x0E : Cursor Location High
// Ce registre contient les 8 bits de poids fort (bits 8 à 15) de la position du curseur sur l'écran.
#define CURSOR_LOCATION_HIGH 0x0E

// I/O basic functions
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

#endif /* KFS_IO_H */
