#include <stdint.h>
#include "utils.h"
#include "screen.h"

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdtr_t;

extern gdtr_t gdtr_var;
extern void read_gdtr(void);

void puthex(uint32_t val)
{
    const char hex[] = "0123456789ABCDEF";
    putstr("0x");  // préfixe hexadécimal

    for (int i = 28; i >= 0; i -= 4) {
        putchar(hex[(val >> i) & 0xF]);
    }
}

void check_gdtr(void)
{
    read_gdtr();               // Remplit gdtr_var
    gdtr_t snapshot = gdtr_var;

    // Affichage pour debug (adapté à ton kernel)
    newline();
    putstr("GDTR limit: ");
    puthex(snapshot.limit);
    newline();
    putstr("GDTR base : ");
    puthex(snapshot.base);
}
