#include "io.h"

// Write a byte to the specified I/O port
inline void outb(uint16_t port, uint8_t value)
{
    /*
    asm volatile (
        "outb %0, %1"     // send byte (outb) from register to port 
        :                 // output var (empty)
        : "a"(value),     // 2 input var :
                          // %0 is replaced by al register (8 bits), storing the value
          "Nd"(port)      // %1 is replaced by an constant (0-255) or dx register (16 bits), sotring the port
    );
    */
  asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Read a byte from the specified I/O port
inline uint8_t inb(uint16_t port)
{
    uint8_t result;
    /*
    asm volatile (
        "inb %1, %0"      // read byte (inb) from port into register
        : "=a" (result)   // output var :
                          // al register (8 bits) storing the result
        : "Nd" (port)     // input var :
                          // %1 is replaced by an constant (0-255) or dx register, storing the port
    );
    */
    asm volatile ("inb %1" : "=a" (result) : "Nd" (port));
    return result;
}

// Check if data is ready in the PS/2 status port
// return 1 if bit 0 of the status port is set (and ready), otherwise return 0
uint8_t is_ps2_data_ready(void)
{
  return inb(PORT_PS2_STATUS) & 0x01; // mask
}

// Read a scancode from the PS/2 data port
uint8_t read_ps2_scancode(void)
{
  return inb(PORT_PS2_DATA);
}