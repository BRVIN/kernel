#include "cmds.h"

static void cmd_exit(void)
{
	isa_exit();
}

static void cmd_reboot(void)
{
	reboot();
}

static void cmd_date(void)
{
	print_date();
}

static void cmd_clear(void)
{
	clear_screen();
}

static void cmd_gdt(void)
{
	cmd_clear();
	gdtr_t gdtr = read_gdtr();
    puthex_tnl("GDTR base : ", gdtr.base);
    puthex_tnl("GDTR limit: ", gdtr.limit);

	extern uint32_t gdt_start, gdt_end;
    puthex_tnl("gdt_start : ", (uint32_t)&gdt_start);
    puthex_tnl("gdt_end   : ", (uint32_t)&gdt_end);

	print_gdt_selectors();
	print_gdt_segments();
}

static void dump_stack(uint32_t nbLinesToPrint)
{
	extern uint32_t stack_top;
	extern uint32_t stack_bottom;

	// EBP Base Pointer = debut de la frame stack de la fonction
	uint32_t *ebp;
	asm volatile("mov %%ebp, %0" : "=r"(ebp));
	puthex_tnl("EBP: ", (uint32_t)ebp);

	uint32_t example1 = 0x42e11111;
	uint32_t example2 = 0x42e22222;
	uint32_t example3 = 0x42e33333;

	// ESP stack pointer = sommet de la pile actuel
    uint32_t *esp;
    asm volatile("mov %%esp, %0" : "=r"(esp));
	puthex_tnl("ESP: ", (uint32_t)esp);

	puthex_t("top: ", (uint32_t)&stack_top);
	puthex_t(" bottom: ", (uint32_t)&stack_bottom);
	putstr(" size: ");
	putnbr_nl((uint32_t)&stack_top - (uint32_t)&stack_bottom);

	putstr_nl("------------------------");
	putstr_nl("addr       | value");
	for (int i = 0; i < nbLinesToPrint; i++)
	{
		uint32_t *addr = esp + i;
		puthex((uint32_t)addr);
		putstr(" : ");
        puthex(*addr);
		if (*addr == *esp) {
			putstr(" <- Current ESP");
		}
		if (*addr == *ebp) {
			putstr(" <- Current EBP");
		}
		newline();
    }
	putstr_nl("------------------------");
}

static void cmd_stk(void)
{
	dump_stack(14);
}

static void cmd_stkt(void)
{
    uint32_t *esp;
    asm volatile("mov %%esp, %0" : "=r"(esp));
	for (int i = 0; i < 3; i++)
	{
		uint32_t *addr = esp + i;
		puthex((uint32_t)addr);
		putstr(" : ");
        puthex_nl(*addr);
	}

    asm volatile(
        "push %0\n"
        "push %1\n"
        "push %2\n"
        :
        : "r"(0x42e11111), "r"(0x42e22222), "r"(0x42e33333)
    );

	putstr("Pushed values: ");
	puthex(0x42e11111);
	putstr(", ");
	puthex(0x42e22222);
	putstr(", ");
	puthex_nl(0x42e33333);

    asm volatile("mov %%esp, %0" : "=r"(esp));
	for (int i = 0; i < 3; i++)
	{
		uint32_t *addr = esp + i;
		puthex((uint32_t)addr);
		putstr(" : ");
        puthex_nl(*addr);
	}

	uint32_t val1, val2, val3;
	asm volatile(
        "pop %0\n"
        "pop %1\n"
        "pop %2\n"
        : "=r"(val3), "=r"(val2), "=r"(val1)
    );

	putstr("Popped values: ");
	puthex(val3);
	putstr(", ");
	puthex(val2);
	putstr(", ");
	puthex_nl(val1);

    asm volatile("mov %%esp, %0" : "=r"(esp));
	for (int i = 0; i < 3; i++)
	{
		uint32_t *addr = esp + i;
		puthex((uint32_t)addr);
		putstr(" : ");
        puthex_nl(*addr);
	}
}

static void cmd_help(void)
{
	cmd_clear();
	putstr_nl("ARMADILLO-1 v0.25 : ELF 32-bit LSB executable, Intel 80386 (x86)");
	putstr_nl("help    : open this help");
    putstr_nl("exit    : shutdown system");
    putstr_nl("reboot  : reboot system");
    putstr_nl("date    : display current date");
    putstr_nl("clear   : clear the terminal screen");
    putstr_nl("gdt     : display GDT address");
    putstr_nl("stk     : stack dump");
    putstr_nl("stkt    : push/pop stack test");
}

void parse_commands(const char *input)
{
	typedef void (*cmd_func)(void);

	typedef struct
	{
		const char *name;
		cmd_func function;
	} KernelCommand;

	KernelCommand kcmds[] = {
		{"help", cmd_help},
		{"exit", cmd_exit},
		{"reboot", cmd_reboot},
		{"date", cmd_date},
		{"clear", cmd_clear},
		{"gdt", cmd_gdt},
		{"stk", cmd_stk},
		{"stkt", cmd_stkt},
	};

	size_t count = sizeof(kcmds) / sizeof(KernelCommand);

	for (size_t i = 0; i < count; i++)
	{
		if (strcmp(input, kcmds[i].name) == 0)
		{
			newline();
			kcmds[i].function();
			return;
		}
	}
}
