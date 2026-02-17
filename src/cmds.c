#include "cmds.h"
#include <stdint.h>

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdtr_t;

extern gdtr_t gdtr_var;
extern void read_gdtr(void);

static void cmd_help(void)
{
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
	newline();
    putstr("gdt   : display GDT address");
	newline();
    putstr("stacktest : push/pop stack test");
}

static inline void __exit(unsigned short port, unsigned short val)
{
	__asm__ volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

static void cmd_exit(void)
{
	//__exit(0x501, 0x2000); // todo check this shit
	__exit(0x604, 0x2000);
	while (1)
	{
		__asm__ volatile("hlt");
	};
}

static void cmd_date(void)
{
	print_date();
}

static void cmd_uname(void)
{
    putstr("myos.bin: ELF 32-bit LSB executable, Intel 80386");
}

static void cmd_whoami(void)
{
    putstr("root");
}

static void cmd_version(void)
{
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

extern uint32_t stack_top;
extern uint32_t stack_bottom;

static void dump_stack(uint32_t nbLinesToPrint)
{
	// stack_bottom	plus petite	Limite basse de la pile
	// ESP	entre les deux	Sommet actuel de la pile (descend avec push)
	// stack_top	plus grande	Sommet initial / ESP initial
	// pile descendante -> stack_bottom < ESP <= stack_top

    uint32_t *esp;
    asm volatile("mov %%esp, %0" : "=r"(esp));

	putstr("------ dump_stack ------");
	newline();
	putstr("Stack bottom: ");
	puthex((uint32_t)&stack_bottom);
	newline();
	putstr("------------------------");
	newline();

	// putstr("Current ESP : "); // Extended Stack Pointer (sommet de la pile)
	// puthex((uint32_t)esp);
	// newline();

	for (int i = 0; i < nbLinesToPrint; i++)
    {
        puthex((uint32_t)(esp + i));
        putstr(" : ");
        puthex(esp[i]);
		if (i == 0) {
			putstr(" -> Current ESP (sommet de la pile)");
		}
        newline();
    }

	putstr("------------------------");
	newline();
	putstr("Stack top (ESP initial): ");
	puthex((uint32_t)&stack_top);
	newline();

}

static void cmd_segments(void)
{
    uint16_t cs, ds, ss; // code / data / stack segments

	asm volatile("mov %%cs, %0" : "=r"(cs));
	asm volatile("mov %%ds, %0" : "=r"(ds));
	asm volatile("mov %%ss, %0" : "=r"(ss));

	putstr("CS:");
	puthex(cs);
	putstr(" DS:");
	puthex(ds);
	putstr(" SS:");
	puthex(ss);
	newline();
}

extern uint32_t gdt_start;
extern uint32_t gdt_end;
extern uint32_t gdt_descriptor;

static void cmd_gdt(void)
{
	read_gdtr(); // asm call
	gdtr_t snapshot = gdtr_var;

    putstr("GDTR limit:");
    puthex(snapshot.limit);
    newline();
    putstr("GDTR base:");
    puthex(snapshot.base); // should be 0x800
    newline();
    putstr("gdt_start:"); // should be 0x800
    puthex((uint32_t)&gdt_start);
    putstr(" gdt_end:");
    puthex((uint32_t)&gdt_end);
    putstr(" gdt_descriptor:");
    puthex((uint32_t)&gdt_descriptor);
    newline();
	cmd_segments();
	dump_stack(8);
}

static void cmd_stacktest(void)
{
    uint32_t a = 0x11111111;
    uint32_t b = 0x22222222;
    uint32_t c = 0x33333333;

    // dump_stack(5);

    asm volatile(
        "push %0\n"
        "push %1\n"
        "push %2\n"
        :
        : "r"(a), "r"(b), "r"(c)
    );

	putstr("Pushed values: ");
	puthex(a);
	putstr(", ");
	puthex(b);
	putstr(", ");
	puthex(c);
	newline();

    dump_stack(16);

    uint32_t val1, val2, val3;
    asm volatile(
        "pop %0\n"
        "pop %1\n"
        "pop %2\n"
        : "=r"(val3), "=r"(val2), "=r"(val1)
    );

	putstr("Popped values: ");
	puthex(val1);
	putstr(", ");
	puthex(val2);
	putstr(", ");
	puthex(val3);
	newline();
}


typedef void (*cmd_func)(void);

typedef struct {
	const char *name;
	cmd_func function;
} KernelCommand;

KernelCommand kcmds[] = {
	{"help", cmd_help},
	{"exit", cmd_exit},
	{"date", cmd_date},
	{"uname", cmd_uname},
	{"whoami", cmd_whoami},
	{"version", cmd_version},
	{"clear", cmd_clear},
	{"gdt", cmd_gdt},
	{"stacktest", cmd_stacktest},
};

void parse_commands(const char *input)
{
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
