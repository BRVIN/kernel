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
//  VGA CRT Controller (CRTC)

Port 0x3D4 : CRTC Address Register  
    can select vga controller register for read/write operation
    first need to define specific index, in second part data can be obtain with 0x3D5 port

Port 0x3D5 : CRTC Data Register  
    can read or write value on register selected with 0x3D4
*/
#define PORT_CRTC_ADDRESS 0x3D4
#define PORT_CRTC_DATA 0x3D5

// Index 0x0F: Cursor Location Low  
// Holds the lower 8 bits (bits 0 to 7) of the cursor position on the screen.  
#define CURSOR_LOCATION_LOW 0x0F  

// Index 0x0E: Cursor Location High  
// Holds the upper 8 bits (bits 8 to 15) of the cursor position on the screen.  
#define CURSOR_LOCATION_HIGH 0x0E  

// I/O basic functions
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

#endif /* KFS_IO_H */
