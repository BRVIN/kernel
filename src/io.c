#include "io.h"

// Write a byte to the specified I/O port
inline void outb(uint16_t port, uint8_t value)
{
  asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Read a byte from the specified I/O port
inline uint8_t inb(uint16_t port)
{
    uint8_t result;
    asm volatile ("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

// Check if data is ready in the PS/2 status port (bit 0 set(1))
uint8_t is_ps2_data_ready(void)
{
  return inb(PORT_PS2_STATUS) & 0x01;
}

// Read a scancode from the PS/2 data port
uint8_t read_ps2_scancode(void)
{
  return inb(PORT_PS2_DATA);
}

unsigned char read_cmos(unsigned char value)
{
    outb(PORT_CMOS_ADDRESS, value);
    return inb(PORT_CMOS_DATA);
}