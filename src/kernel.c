#include "kernel.h"

extern void kernel_main(void);

void kernel_main(void)
{
	init_screen();

	while (true)
	{
		if (is_ps2_data_ready())
		{
			handle_scancode();
		}
	}
}
