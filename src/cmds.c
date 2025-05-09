#include "cmds.h"

void cmd_help(void)
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
}

static inline void __exit(unsigned short port, unsigned short val)
{
	__asm__ volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

void cmd_exit(void)
{
	//__exit(0x501, 0x2000);
	__exit(0x604, 0x2000);
	while (1)
	{
		__asm__ volatile("hlt");
	};
}

void cmd_date(void)
{
	newline();
	print_date();
}

void cmd_uname(void)
{
	newline();
    putstr("myos.bin: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), statically linked, with debug_info, not stripped");
}

void cmd_whoami(void)
{
	newline();
    putstr("root");
}

void cmd_version(void)
{
	newline();
	putstr("version 0.2 - Compiled on ");
	putstr(__DATE__);
	putstr(" at ");
	putstr(__TIME__);
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
}