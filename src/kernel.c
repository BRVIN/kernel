#include "kernel.h"

void test_gdt_segments()
{
	// Accéder au segment de données (index 2 dans la GDT)
	asm volatile("mov $0x10, %ax"); // Charger le sélecteur de segment de données
	asm volatile("mov %ax, %ds");   // Charger le segment de données dans DS
	asm volatile("mov %ax, %es");   // Charger le segment de données dans ES
	asm volatile("mov %ax, %fs");   // Charger le segment de données dans FS
	asm volatile("mov %ax, %gs");   // Charger le segment de données dans GS
	asm volatile("mov %ax, %ss");   // Charger le segment de données dans SS

	// Effectuer une opération avec le segment de données
	uint32_t *ptr = (uint32_t *)0x1000; // Par exemple, une adresse de la pile
	*ptr = 0xDEADBEEF;                  // Si cela passe sans exception, cela signifie que l'accès fonctionne.

	// Accéder au segment de code (index 1 dans la GDT)
	asm volatile("mov $0x08, %ax"); // Charger le sélecteur de segment de code
	asm volatile("mov %ax, %cs");   // Charger le segment de code dans CS
}

/*
   void test_protected_mode_transition() {
// Passage en mode protégé (mode 32 bits)
asm volatile(
"mov $0x10, %ax\n\t"  // Charger le sélecteur de données dans %ax
"mov %ax, %ds\n\t"     // Charger le segment de données dans DS
"mov %ax, %es\n\t"     // Charger le segment de données dans ES
"mov %ax, %ss\n\t"     // Charger le segment de données dans SS
"mov $0x08, %ax\n\t"   // Charger le sélecteur de code dans %ax
"mov %ax, %cs\n\t"     // Charger le segment de code dans CS
"ljmp $0x08, $.1\n\t"  // Jump à une adresse protégée dans le segment de code
".1:\n\t"
"nop\n\t"              // Une fois le mode protégé activé, le programme continue ici
);
}
 */

void test_gdt_access()
{
	// Segments valides (assumé par le sélecteur 0x08 et 0x10 pour le code et les données)

	// Tester l'accès au segment de données
	uint32_t *data_segment = (uint32_t *)0x1000; // Adresse dans le segment de données
	*data_segment = 0xDEADBEEF;                  // Test de lecture/écriture
	uint32_t value = *data_segment;              // Lire la valeur
	if (value != 0xDEADBEEF)
	{
		// Si cela échoue, la GDT ou les segments sont probablement mal configurés
	}

	// Tester l'accès au segment de code (ne pas essayer d'écrire, car c'est un segment en lecture seule)
	uint32_t *code_segment = (uint32_t *)0x2000;
	// Essayer de lire dans un segment de code, cela devrait être possible
	value = *code_segment; // Si cela échoue, il y a un problème avec la configuration
}

void enter_protected_mode()
{
	// Désactiver les interruptions
	asm volatile("cli");

	// Activer le mode protégé
	asm volatile(
			"mov %%cr0, %%eax\n\t" // Charger CR0 dans eax
			"or $0x1, %%eax\n\t"   // Activer le bit PE (Protection Enable)
			"mov %%eax, %%cr0\n\t" // Écrire de nouveau dans CR0
			::: "eax");

	// Recharger les registres de segments
	asm volatile(
			"mov $0x10, %ax\n\t" // Sélecteur de données
			"mov %ax, %ss\n\t"   // Charger SS
			"mov %ax, %ds\n\t"   // Charger DS
			"mov %ax, %es\n\t"   // Charger ES
			"mov %ax, %fs\n\t"   // Charger FS
			"mov %ax, %gs\n\t"   // Charger GS
		    );

	// Passer à un code dans le segment de code protégé
	asm volatile("ljmp $0x08, $protected_code\n\t");

	// Code protégé
	asm volatile(
			"protected_code:\n\t"
			"nop\n\t" // Point d'entrée du code protégé
		    );
	asm volatile("sti");
}

void update_screen(void)
{
	show_tabs();
	for (size_t i = 0; i < (VGA_HEIGHT * VGA_WIDTH); ++i)
	{
		g_buffer[i] = g_screens[g_current_screen][i];
	}
}


void kernel_main(void)
{
	asm volatile("cli");
	// init_gdt(); // KFS2

	init_screen();
	show_42();
	update_screen();
	bool welcome_logo = true;
	move_cursor(1, 3);
	while (true)
	{
		if (is_ps2_data_ready())
		{
			if (welcome_logo == true)
			{
				welcome_logo = false;
				remove_logo();
			}
			handle_scancode();
			update_screen();
		}
	}
	// KFS2
	// enter_protected_mode();
	// test_gdt_segments();
	// test_protected_mode_transition();
	// test_gdt_access();
}
