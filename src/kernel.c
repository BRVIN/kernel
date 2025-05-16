#include "kernel.h"

void kernel_main(void)
{
	init_screen();
	int welcome_logo = 1;
  int nb = 10;
	while (1)
	{
		if (is_ps2_data_ready())
		{
			if (welcome_logo == 1 && nb == 0)
			{
				welcome_logo = 0;
				remove_logo();
			}
    nb--
			handle_scancode();
		}
	}
}
