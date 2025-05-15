#include "utils.h"

int isprint(const char c)
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

void putstr(const char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
		putchar(str[i]);
}

void	putnbr_at(int n, uint8_t color, int x, int y)
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

void puthex_at(const uint8_t scancode, const int x, const int y)
{
	static const char hex_chars[] = "0123456789ABCDEF";
	uint8_t high_nibble = (scancode >> 4) & 0x0F; // Récupère les 4 bits de poids fort
	uint8_t low_nibble = scancode & 0x0F;         // Récupère les 4 bits de poids faible

	// Affiche les deux caractères hexadécimaux
	putchar_at(hex_chars[high_nibble], VGA_COLOR_GREEN, x, y);
  putchar_at(hex_chars[low_nibble], VGA_COLOR_GREEN, x + 1, y);
}

void putcoord(const int cx, const int cy, uint8_t color, const int x, const int y)
{
	putstr_at("(", color, x, y);
	putnbr_at(cx, color, x + 2, y);
	putstr_at(",", color, x + 3, y);
	putnbr_at(cy, color, x + 5, y);
	putstr_at(")", color, x + 6, y);
}

int contains(char c, const char *str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
		{
			return 1;
		}
		i++;
	}
	return 0;
}
