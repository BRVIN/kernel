#include "gdt.h"
#include "utils.h"

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

    // TODO add 4 others entry
    /*
    Null
    Kernel Code (code exécutable en mode noyau, ring 0)
    Kernel Data (données en mode noyau)
    Kernel Stack (pile du noyau)

    User Code (code exécutable en mode utilisateur, ring 3)
    User Data (données utilisateur)
    User Stack (pile utilisateur)
    */

    /* v2*/
    // asm volatile("mov %0, %%eax" : : "r"(&gdt_ptr));
    //
    // gdt_verify();

    // load_gdt(&gdt_ptr);

    // gdt_verify();

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

// Test simple des segments : lecture/écriture dans chaque segment
void test_segment(uint16_t selector, const char* name)
{
    putstr("Testing segment: ");
    putstr(name);
    putstr(" ... ");

    putstr(" OFF ");
    return;

    volatile int test_var = 0;

    asm volatile (
        "mov %%ax, %%ds \n\t"
        "movl $0x12345678, %0 \n\t"
        : "=m"(test_var)
        : "a"(selector)
    );

    // Si on atteint cette ligne sans GPF, le segment fonctionne
    putstr("OK\n");
}