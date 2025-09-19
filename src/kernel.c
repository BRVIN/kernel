#include "kernel.h"

extern void gdt_flush(void);
void kernel_main(void)
{
	asm volatile("cli");

	// init_gdt();
    // gdt_flush(); // load gdt at 0x800

	init_screen();

	bool welcome_logo = true;
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
		}

		// check_gdtr();
	}
}
