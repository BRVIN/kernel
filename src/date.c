#include "date.h"

static unsigned char bcd_to_bin(unsigned char bcd)
{
    return (bcd & 0x0F) + ((bcd >> 4) * 10);
}

static void get_system_time(int *hour, int *minute, int *second)
{
    *second = bcd_to_bin(read_cmos(0x00)); // Secondes
    *minute = bcd_to_bin(read_cmos(0x02)); // Minutes
    *hour = bcd_to_bin(read_cmos(0x04));   // Heures
}

static void get_system_date(int *day, int *month, int *year)
{
    *day = bcd_to_bin(read_cmos(0x07));   // Jour
    *month = bcd_to_bin(read_cmos(0x08)); // Mois
    *year = bcd_to_bin(read_cmos(0x09));  // Année (sur 2 chiffres)
}

static void print_2digits(int value)
{
    putchar('0' + (value / 10));
    putchar('0' + (value % 10));
}

// Convertit un entier en chaîne (4 chiffres)
static void print_4digits(int value)
{
    putchar('0' + (value / 1000) % 10);
    putchar('0' + (value / 100) % 10);
    putchar('0' + (value / 10) % 10);
    putchar('0' + (value % 10));
}

void print_date(void)
{
    int d, m, y, h, min, s;
    get_system_date(&d, &m, &y);
    get_system_time(&h, &min, &s);

    // year fix
    y += 2000;
    //hour fix (+2 heures)
    h = (h + 2) % 24;

    print_2digits(d);
    putchar('/');
    print_2digits(m);
    putchar('/');
    print_4digits(y);
    putchar(' ');
    print_2digits(h);
    putchar(':');
    print_2digits(min);
    putchar(':');
    print_2digits(s);
}