#include "keyboard.h"

#define SCODE_CTRL_PRESSED 0x1D
#define SCODE_CTRL_RELEASED 0x9D
#define SCODE_SHIFT_PRESSED 0x2A
#define SCODE_SHIFT_RELEASED 0xAA
#define SCODE_BACKSPACE 0x0E
#define SCODE_ENTER 0x1C
#define SCODE_TAB 0x0F
#define SCODE_ESCAPE 0x01
#define PRESSED 1
#define RELEASED 0

char input[128] = { 0 };
uint8_t bufid = 0;

void ctrl_key_handler(char key)
{
	// changement d'ecran
	if (key >= '1' && key <= '3')
		g_current_screen = key - 49;

	// changement de couleur (variable g_color)
	if (key == '4' || key == '$')
		screen_yellow();
	if (key == '5' || key == '%')
		screen_magenta();
	if (key == '6' || key == '^')
		screen_green();
	if (key == '4' || key == '5' || key == '6')
		recolor_screen_at(get_cursor_pos());
	if (key == '$' || key == '%' || key == '^')
		recolor_screen_at(0);
	return;
}

void clear_input() {
	for (int i = 0; i < 128; i++) {
		input[i] = '\0';
	}
	bufid = 0;
}

void goto_next_screen(void)
{
	g_current_screen++;
	if (g_current_screen == 3)
		g_current_screen = 0;
}

void ft_delete_last_entry(void)
{
	if (g_x[g_current_screen] < 2)
		return;
	g_x[g_current_screen]--;

	uint16_t cursor_pos = get_cursor_pos();
	uint16_t cursor_x = get_cursor_x(cursor_pos);
	uint16_t cursor_y = get_cursor_y(cursor_pos);
	uint16_t x = cursor_x - 1;
	uint16_t y = cursor_y;
	const size_t pos_to_erase = y * VGA_WIDTH + x;
  	g_screens[g_current_screen][pos_to_erase] = EMPTY_VGA;

	if (bufid > 0)
		bufid--;
	input[bufid] = '\0';
}

void handle_scancode(void)
{
	static int shift_state = 0;
	static int ctrl_state = 0;

	uint8_t scancode = read_ps2_scancode();

	if (scancode == SCODE_CTRL_PRESSED)
	{
		ctrl_state = PRESSED;
		return ;
	}
	else if (scancode == SCODE_CTRL_RELEASED)
	{
		ctrl_state = RELEASED;
		return ;
	}
	else if (scancode == SCODE_SHIFT_PRESSED)
	{
		shift_state = PRESSED;
		return ;
	}
	else if (scancode == SCODE_SHIFT_RELEASED)
	{
		shift_state = RELEASED;
		return ;
	}

	char user_input = scancode_table[scancode][shift_state];

	if (ctrl_state == PRESSED)
	{
		ctrl_key_handler(user_input);
		return ;
	}
	if (scancode == SCODE_ENTER) {
		input[bufid] = '\0';
		parse_commands(input);
		newline();
		display_prompt();
		clear_input();
	}
	else if (scancode == SCODE_TAB) {
		goto_next_screen();
	}
	else if (scancode == SCODE_BACKSPACE) {
		ft_delete_last_entry();
	}
	else if (isprint(user_input))
	{
		putchar(user_input);
		input[bufid] = user_input;
		bufid++;
	}
	move_cursor(g_x[g_current_screen], g_y[g_current_screen]);
	for (size_t i = 0; i < (VGA_HEIGHT * VGA_WIDTH); ++i)
	{
		g_buffer[i] = g_screens[g_current_screen][i];
	}
	// DEBUG HEADER
	draw_reset_top_bar();
	draw_tabs();
	draw_dbg_input(input);
	draw_dbg_cursor_pos();
	draw_dbg_scancode(scancode);
}
