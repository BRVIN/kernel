#include "printk.h"

static char *itoa(int num, char *str, int base)
{
	int i = 0;
	int isNegative = 0;

	// Si le nombre est 0
	if (num == 0)
	{
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	// Si le nombre est négatif, ajouter un signe et rendre le nombre positif
	if (num < 0 && base == 10)
	{
		isNegative = 1;
		num = -num;
	}

	// Convertir chaque chiffre
	while (num != 0)
	{
		int rem = num % base;
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num / base;
	}

	// Ajouter le signe pour les nombres négatifs
	if (isNegative)
	{
		str[i++] = '-';
	}

	str[i] = '\0'; // Terminer la chaîne

	// Inverser la chaîne pour la mettre dans le bon ordre
	int start = 0;
	int end = i - 1;
	while (start < end)
	{
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}

	return str;
}

static int vsprintf(char *buf, const char *fmt, va_list args)
{
	int i = 0;
	const char *ptr;
	char temp[32]; // Tampon pour les conversions

	for (ptr = fmt; *ptr != '\0'; ptr++)
	{
		if (*ptr == '%')
		{
			ptr++;
			if (*ptr == 'd')
			{
				int num = va_arg(args, int);
				itoa(num, temp, 10); // Convertir l'entier en chaîne
				for (int j = 0; temp[j] != '\0'; j++, i++)
				{
					buf[i] = temp[j];
				}
			}
			else if (*ptr == 's')
			{
				char *str = va_arg(args, char *);
				for (int j = 0; str[j] != '\0'; j++, i++)
				{
					buf[i] = str[j];
				}
			}
			else
			{
				// Si on rencontre un format non pris en charge, on l'ajoute tel quel
				buf[i++] = *ptr;
			}
		}
		else
		{
			// Ajouter le caractère normal au tampon
			buf[i++] = *ptr;
		}
	}
	buf[i] = '\0'; // Terminer la chaîne

	return i;
}

int printk(const char *fmt, ...)
{
	va_list args;
	char buf[256]; // Le tampon de sortie

	va_start(args, fmt);
	vsprintf(buf, fmt, args);

	// Par exemple, utiliser une interface de port série ou une sortie directe à l'écran
	// Pour l'instant, affichage simple sur la sortie système standard
	// (en général, tu devras utiliser une méthode spécifique à ton noyau pour sortir les données)
	// Elle pourrait écrire sur un port série, la mémoire vidéo, ou une autre méthode
	// Utilise un printf simple, mais il faudra remplacer par une vraie implémentation
	////ft_putstr(buf);

	va_end(args);
	return 0;
}
