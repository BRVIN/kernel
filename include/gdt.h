#ifndef KFS_GDT_H
#define KFS_GDT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* gdt */
void init_gdt(void);


// Define a GDT entry
struct GDTEntry
{
    uint16_t limit_low;  // Lower 16 bits of the limit
    uint16_t base_low;   // Lower 16 bits of the base
    uint8_t base_middle; // Next 8 bits of the base
    uint8_t access;      // Access flags
    uint8_t granularity; // Granularity and higher 4 bits of limit
    uint8_t base_high;   // Last 8 bits of the base
} __attribute__((packed));

// Define a pointer to the GDT // (=) GDTR
struct GDTPointer
{
    uint16_t limit; // Size of the GDT (bytes - 1)
    uint32_t base;  // Address of the first GDT entry
} __attribute__((packed));
// évite que le compilateur ajoute du padding dans la structure.

// Declare GDT and its pointer
// struct GDTEntry gdt[3];
// struct GDTPointer gdt_ptr;

void test_segment(uint16_t selector, const char* name);

#endif /* KFS_GDT_H */
