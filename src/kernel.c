#include "kernel.h"

void kernel_main(void)
{
	init_screen();

  while (1)
  {
    if (is_ps2_data_ready())
      handle_scancode();
  }
}
