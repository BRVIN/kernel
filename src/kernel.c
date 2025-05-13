#include "kernel.h"

void kernel_main(void)
{
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
	}
}
