#include "gdt.h"

extern void load_gdt(struct GDTPointer *); // Assembly function
//
extern void gdt_verify(void); // Assembly function
//
// extern void load_gdt();   // Assembly function

// struct GDTEntry gdt[3];
// struct GDTPointer gdt_ptr;

struct GDTEntry gdt[3] __attribute__((aligned(8))) = {0}; // Définir la GDT à l'adresse voulue
struct GDTPointer gdt_ptr = {sizeof(gdt) - 1, (uint32_t)&gdt};

// Function to set an entry in the GDT
void set_gdt_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    gdt[index].base_low = base & 0xFFFF;
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;
    gdt[index].limit_low = limit & 0xFFFF;
    gdt[index].granularity = (limit >> 16) & 0x0F;
    gdt[index].granularity |= (granularity & 0xF0);
    gdt[index].access = access;
}

// Initialize the GDT
void init_gdt()
{
    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base = (uint32_t)&gdt;

    set_gdt_entry(0, 0, 0, 0, 0);             // Null segment
    set_gdt_entry(1, 0, 0xFFFFF, 0x9A, 0xCF); // Code segment
    set_gdt_entry(2, 0, 0xFFFFF, 0x92, 0xCF); // Data segment

    /* v2*/
    // asm volatile("mov %0, %%eax" : : "r"(&gdt_ptr));
    //
    gdt_verify();

    load_gdt(&gdt_ptr);

    gdt_verify();

    /* v1 ok  */
    /*
        // Charger la GDT
        asm volatile("lgdt %0" : : "m"(gdt_ptr));
        // Activation de la protection de mémoire et du mode protégé
        asm volatile("mov $0x10, %ax; mov %ax, %ds");
        asm volatile("mov $0x10, %ax; mov %ax, %es");
        asm volatile("mov $0x10, %ax; mov %ax, %ss");
        asm volatile("ljmp $0x08, $.1");
        asm volatile(".1:");
    */
}
