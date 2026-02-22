#include "io.h"

/*
    I/O matériel x86 direct CPU access
    >> Pour communiquer avec le cpu
*/
// Write a byte (8 bits) to the specified I/O port
inline void outb(uint16_t port, uint8_t value)
{
  asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Write 16bits val dans le port I/O port du cpu
inline void outw(unsigned short value, unsigned short port)
{
  asm volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}

// Read a byte from the specified I/O port
inline uint8_t inb(uint16_t port)
{
  uint8_t result;
  asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
  return result;
}

/*
    PS/2 controller ports
    >> Pour la detection des keyboard inputs
*/
// Check if data is ready in the PS/2 status port (bit 0 set(1))
inline uint8_t is_ps2_data_ready(void)
{
  return inb(PORT_PS2_STATUS) & 0x01;
}

// Read a scancode from the PS/2 data port
inline uint8_t read_ps2_scancode(void)
{
  return inb(PORT_PS2_DATA);
}

/*
  CMOS
  >> Pour recuperer la date et l'heure
*/
inline unsigned char read_cmos(unsigned char value)
{
  outb(PORT_CMOS_ADDRESS, value);
  return inb(PORT_CMOS_DATA);
}

/*
    QEMU périphérique isa-debug-exit card
    >> Pour quitter / reboot
*/
inline void isa_exit()
{
  // 0x2000 valeur magique pour quitter QEMU
  // 0x604 port isa debug
  outw(0x2000, 0x604);
  while (1)
  {
    __asm__ volatile("hlt");
  }
}

inline void reboot()
{
  // 0xFE dans le port 0x64
  // (envoie la commande "pulse reset" controller clavier 8042)
  outb(0x64, 0xFE);
  while (1)
  {
    __asm__ volatile("hlt");
  }
}

/*
  GDT
*/

inline gdtr_t read_gdtr(void)
{
  gdtr_t gdtr;
  asm volatile("sgdt %0" : "=m"(gdtr));
  return gdtr;
  // sgdt : instruction x86 qui lit le GDTR, on le stocke dans gdtr_var
  // lgdt : charge le GDTR dans le CPU depuis une adresse mémoire (struct { limit; base }
  // GDTR : registre special (Global Descriptor Table Register)
  // usage : pour savoir ou se trouve la GDT et sa taille
}

// selecteur code segment
inline uint16_t get_cs(void)
{
  uint16_t cs;
  asm volatile("mov %%cs, %0" : "=r"(cs));
  return cs;
}

// selecteur data segment
inline uint16_t get_ds(void)
{
  uint16_t ds;
  asm volatile("mov %%ds, %0" : "=r"(ds));
  return ds;
}

// selecteur stack segment
inline uint16_t get_ss(void)
{
  uint16_t ss;
  asm volatile("mov %%ss, %0" : "=r"(ss));
  return ss;
}

// index dans la GDT
inline uint16_t get_index(uint16_t selector)
{
    return selector >> 3;
    // # selecteur sur 16 bits
    // # 15         3  2  1 0
    // # +------------+--+--+
    // # |   Index    |TI|RPL|
    // # +------------+--+--+
}

// DPL (from gdt): Descrptor Privilege Level du segment (ring 0 / 3)
inline uint8_t get_dpl(uint16_t index)
{
	  extern uint32_t gdt_start;
    uint64_t descriptor = ((uint64_t*)&gdt_start)[index];
    return (descriptor >> 45) & 0x3; // bit 45-46 = DPL
}
