#include "utils.h"

bool isprint(const char c)
{
	return c >= 32 && c <= 126;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (unsigned char)(*s1) - (unsigned char)(*s2);
}

size_t strlen(const char *str)
{
    size_t i = 0;

    while (str[i])
    {
        i++;
    }
    return i;
}

/* PUTSTR */

void putstr(const char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
		putchar(str[i]);
}

void putstr_nl(const char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
		putchar(str[i]);
	newline();
}

/* PUTNBR */

void putnbr(int n)
{
	if (n >= 0)
	{
		if (n > 9)
		{
      putnbr(n / 10);
    }
		putchar(n % 10 + 48);
	}
	else if (n == -2147483648)
		putstr("-2147483648");
	else
	{
		putchar('-');
    putnbr(-n);
  }
}

void putnbr_nl(int n)
{
	putnbr(n);
	newline();
}

void putnbr_at(int n, uint8_t color, int x, int y)
{
	if (n >= 0)
	{
		if (n > 9)
		{
      putnbr_at(n / 10, color, x - 1, y);
    }
		putchar_at(n % 10 + 48, color, x, y);
	}
	else if (n == -2147483648)
		putstr_at("-2147483648", color, x, y);
	else
	{
		putchar_at('-', color, x, y);
    putnbr_at(-n, color, x + 1, y);
  }
}

/* PUTHEX */

void puthex(uint32_t val)
{
    const char hex[] = "0123456789ABCDEF";
    putstr("0x");  // préfixe hexadécimal

    for (int i = 28; i >= 0; i -= 4) {
        putchar(hex[(val >> i) & 0xF]);
    }
}

void puthex_nl(uint32_t val)
{
	puthex(val);
	newline();
}

void puthex_t(const char *str, uint32_t val)
{
	putstr(str);
	puthex(val);
}

void puthex_tnl(const char *str, uint32_t val)
{
	putstr(str);
	puthex(val);
	newline();
}

void puthex_at(const uint8_t scancode, const int x, const int y)
{
	static const char hex_chars[] = "0123456789ABCDEF";
	uint8_t high_nibble = (scancode >> 4) & 0x0F; // Récupère les 4 bits de poids fort
	uint8_t low_nibble = scancode & 0x0F;         // Récupère les 4 bits de poids faible

	// Affiche les deux caractères hexadécimaux
	putchar_at(hex_chars[high_nibble], VGA_COLOR_GREEN, x, y);
  putchar_at(hex_chars[low_nibble], VGA_COLOR_GREEN, x + 1, y);
}

/* SCREEN COORD */

void putcoord(const int cx, const int cy, uint8_t color, const int x, const int y)
{
	putstr_at("(", color, x, y);
	putnbr_at(cx, color, x + 2, y);
	putstr_at(",", color, x + 3, y);
	putnbr_at(cy, color, x + 5, y);
	putstr_at(")", color, x + 6, y);
}

/* GDT */

static void print_selector(uint16_t selector, const char *name)
{
	uint16_t index = get_index(selector);
	uint8_t dpl = get_dpl(index);
	puthex(selector);
	putstr(" | ");
	putnbr(index);
	putstr("  | ");
	putnbr(dpl);
	putstr("    | ");
	putstr_nl(name);
}

void print_gdt_selectors()
{
	putstr_nl("----- GDT selectors -----");
	putstr_nl("addr       | id | ring | selector");
	print_selector(get_cs(), "CS");
	print_selector(get_ds(), "DS");
	print_selector(get_ss(), "SS");
}

void print_gdt_segments()
{
	putstr_nl("----- GDT segments -----");
	putstr_nl("addr       | id | ring");
    for (int i = 0; i <= 6; i++)
    {
		puthex(i * 0x8);
		putstr(" | ");
		putnbr(i);
		putstr("  | ");
		putnbr(get_dpl(i));
		newline();
	}
}