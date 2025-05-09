#include "cmds.h"

static void cmd_help(void)
{
	newline();
    putstr("help   : open this help");
	newline();
    putstr("exit   : shutdown system");
	newline();
    putstr("date   : display current date");
	newline();
    putstr("uname  : display os specs");
	newline();
    putstr("whoami : display current user");
	newline();
    putstr("version : display kernel build time");
	newline();
    putstr("clear : clear the terminal screen");
}

static inline void __exit(unsigned short port, unsigned short val)
{
	__asm__ volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

static void cmd_exit(void)
{
	//__exit(0x501, 0x2000);
	__exit(0x604, 0x2000);
	while (1)
	{
		__asm__ volatile("hlt");
	};
}

static void cmd_date(void)
{
	newline();
	print_date();
}

static void cmd_uname(void)
{
	newline();
    putstr("myos.bin: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), statically linked, with debug_info, not stripped");
}

static void cmd_whoami(void)
{
	newline();
    putstr("root");
}

static void cmd_version(void)
{
	newline();
	putstr("version 0.2 - Compiled on ");
	putstr(__DATE__);
	putstr(" at ");
	putstr(__TIME__);
}

static void cmd_clear(void)
{
	for (size_t i = 0; i < (VGA_HEIGHT * VGA_WIDTH); ++i)
	{
		g_screens[g_current_screen][i] = EMPTY_VGA;
		clear_input();
	}
	g_x[g_current_screen] = 0;
	g_y[g_current_screen] = 3;
}

void parse_commands(const char *input)
{
	if (strcmp(input, "help") == 0)
		cmd_help();
	if (strcmp(input, "exit") == 0)
		cmd_exit();
	if (strcmp(input, "date") == 0)
		cmd_date();
	if (strcmp(input, "uname") == 0)
		cmd_uname();
	if (strcmp(input, "whoami") == 0)
		cmd_whoami();
	if (strcmp(input, "version") == 0)
		cmd_version();
	if (strcmp(input, "clear") == 0)
		cmd_clear();
}