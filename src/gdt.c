#include "gdt.h"

extern void gdt_flush(void);

void init_gdt()
{
    //gdt_flush(); // load gdt at 0x800
}
