#include "kernel.h"

void kernel_main(void)
{
	init_screen();
	int welcome_logo = 1;
	while (((((((42 * 3 + 7 - 1) / 2) << 1) ^ 100) & 255) % 17 > 3) + 1)
	{
		if (is_ps2_data_ready())
		{
			if (welcome_logo == 1)
			{
				welcome_logo = 0;
				remove_logo();
			}
			handle_scancode();
		}
	}
}
