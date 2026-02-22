#include "screen.h"

const size_t VGA_WIDTH = 80;  // 80;
const size_t VGA_HEIGHT = 25; // 25;

size_t g_y[3] = {3, 3, 3};
size_t g_x[3] = {0, 0, 0};
uint8_t g_color[3];

uint16_t *g_buffer;
uint16_t g_screens[3][2000];
uint8_t g_current_screen = 0;

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
  return fg | bg << 4;
}

static inline int get_gc_pos(void)
{
  return (g_y[g_current_screen] * VGA_WIDTH + g_x[g_current_screen]);
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
  return (uint16_t)uc | (uint16_t)color << 8;
}

static void auto_scroll(void)
{
  g_x[g_current_screen] = 0;
  g_y[g_current_screen] = VGA_HEIGHT - 1;

  size_t pos_src;
  size_t pos_dst;

  for (size_t yy = 1; yy < VGA_HEIGHT; ++yy)
  {
    for (size_t xx = 0; xx < VGA_WIDTH; ++xx)
    {
      pos_dst = (yy - 1) * VGA_WIDTH + xx;
      pos_src = yy * VGA_WIDTH + xx;
      g_screens[g_current_screen][pos_dst] = g_screens[g_current_screen][pos_src];
    }
  }
  for (size_t x = 0; x < VGA_WIDTH; ++x)
    putchar_at(EMPTY_CHAR, EMPTY_COLOR, x, VGA_HEIGHT - 1);
}

/* PUTS */

void putchar(const char c)
{
  g_screens[g_current_screen][get_gc_pos()] = vga_entry(c, g_color[g_current_screen]);
  g_x[g_current_screen]++;
  if (g_x[g_current_screen] == VGA_WIDTH)
  {
    g_x[g_current_screen] = 0;
    g_y[g_current_screen]++;
    if (g_y[g_current_screen] == VGA_HEIGHT)
    {
      auto_scroll();
    }
  }
}

void putchar_at(const char c, uint8_t color, size_t x, size_t y)
{
    const size_t pos = y * VGA_WIDTH + x;
    g_screens[g_current_screen][pos] = vga_entry(c, color);
}

void putstr_at(const char *str, uint8_t color, size_t ox, size_t y)
{
  for (size_t x = 0; str[x] != '\0'; x++)
  {
    const size_t pos = y * VGA_WIDTH + (ox + x);
    g_screens[g_current_screen][pos] = vga_entry(str[x], color);
  }
}

/* COLORS */

void screen_green(void)
{
  g_color[g_current_screen] = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
}

void screen_yellow(void)
{
  g_color[g_current_screen] = vga_entry_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
}

void screen_magenta(void)
{
  g_color[g_current_screen] = vga_entry_color(VGA_COLOR_MAGENTA, VGA_COLOR_BLACK);
}

void screen_color(enum vga_color fg, enum vga_color bg)
{
  g_color[g_current_screen] = vga_entry_color(fg, bg);
}

//recolore entierement l ecran actuel avec le nouveau g_color
void recolor_screen_at(uint16_t start_pos) {

  char c;
  for (size_t i = start_pos; i < (VGA_HEIGHT * VGA_WIDTH); ++i)
  {
      c = g_screens[g_current_screen][i];
      g_screens[g_current_screen][i] = vga_entry(c, g_color[g_current_screen]);
  }
}

static void recolor_cursor(void)
{
  uint16_t cursor_pos = get_cursor_pos();
  char c = g_screens[g_current_screen][cursor_pos];
  g_screens[g_current_screen][cursor_pos] = vga_entry(c, g_color[g_current_screen]);
}

static void ft_put_entry_empty(void)
{
  const size_t pos = g_y[g_current_screen] * VGA_WIDTH + g_x[g_current_screen];
  g_screens[g_current_screen][pos] = EMPTY_VGA ;
}

void display_prompt(void)
{
  const uint8_t saved = g_color[g_current_screen];
  g_color[g_current_screen] = VGA_COLOR_WHITE;
  putchar('~');
  putchar(' ');
  g_color[g_current_screen] = saved;
}

void newline(void)
{
    g_x[g_current_screen] = 0;
    g_y[g_current_screen]++;
    if (g_y[g_current_screen] == VGA_HEIGHT)
      auto_scroll();
}

/* INIT */

// Fonction pour configurer une taille texte personnalisée
static void set_vga_text_mode(const size_t width, const size_t height)
{
  // Adresse des registres du contrôleur VGA
  const uint16_t VGA_CRTC_INDEX = 0x3D4; // Sélecteur de registre
  const uint16_t VGA_CRTC_DATA = 0x3D5;  // Données du registre

  // Désactiver l'écran pendant la configuration
  outb(VGA_CRTC_INDEX, 0x17);
  outb(VGA_CRTC_DATA, 0x00);

  // Modifier la largeur (nombre de colonnes) à 20
  outb(VGA_CRTC_INDEX, 0x01);          // Registre Horizontal Display End
  outb(VGA_CRTC_DATA, width * 18 - 1); // Colonnes - 1

  // Modifier la hauteur (nombre de lignes) à 10
  outb(VGA_CRTC_INDEX, 0x12);           // Registre Vertical Display End
  outb(VGA_CRTC_DATA, height * 16 - 1); // Lignes - 1

  // Réactiver l'écran
  outb(VGA_CRTC_INDEX, 0x17);
  outb(VGA_CRTC_DATA, 0x80);
}

void remove_logo(void)
{
  for (size_t y = 0; y < 25; y++)
  {
    for (size_t x = 0; x < 80; x++)
    {
      const size_t id = y * VGA_WIDTH + x;
      g_screens[g_current_screen][id] = EMPTY_VGA;
    }
  }
}

void init_screen(void)
{
  set_vga_text_mode(VGA_WIDTH, VGA_HEIGHT);
  g_buffer = (uint16_t *)0xB8000;

  for (size_t y = 0; y < VGA_HEIGHT; y++)
  {
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
      const size_t id = y * VGA_WIDTH + x;
      g_buffer[id] = EMPTY_VGA;

      for (int i = 0; i < 3; ++i)
        g_screens[i][id] = EMPTY_VGA;
    }
  }

  g_current_screen = 2;
  screen_yellow();
  display_prompt();

  g_current_screen = 1;
  screen_magenta();
  display_prompt();

  g_current_screen = 0;
  screen_green();

	draw_42_logo();

	for (size_t i = 0; i < (VGA_HEIGHT * VGA_WIDTH); ++i)
  {
    g_buffer[i] = g_screens[g_current_screen][i];
  }

  while (true)
  {
    if (is_ps2_data_ready())
    {
      read_ps2_scancode();
      break;
    }
  }
  while (true)
  {
    if (is_ps2_data_ready())
    {
      read_ps2_scancode();
      break;
    }
  }

  remove_logo();
  display_prompt();

	move_cursor(1, 3);
	draw_reset_top_bar();
	draw_tabs();
	for (size_t i = 0; i < (VGA_HEIGHT * VGA_WIDTH); ++i)
  {
    g_buffer[i] = g_screens[g_current_screen][i];
  }
}


/* CURSOR */

/* VGA CRT Controller (CRTC)
Port 0x3D4 : CRT Controller Index Register (CRTC Address Register)
    Ce port est utilisé pour sélectionner l'index du registre CRT que l'on souhaite lire ou écrire.
    Il agit comme un sélecteur de registre parmi les registres internes du contrôleur CRT.
    Une fois l'index défini, la lecture/écriture se fait via le port 0x3D5.

Port 0x3D5 : CRT Controller Data Register (CRTC Data Register)
    Ce port est utilisé pour lire ou écrire la valeur dans le registre CRT sélectionné via 0x3D4.
    En d'autres termes, après avoir défini l'index sur le port 0x3D4, ce port permet d'accéder aux données du registre correspondant.
*/

void move_cursor(const int x, const int y)
{
	uint16_t pos = y * VGA_WIDTH + x;

	outb(PORT_CRTC_ADDRESS, CURSOR_LOCATION_LOW);
	outb(PORT_CRTC_DATA, (uint8_t) (pos & 0xFF));
	outb(PORT_CRTC_ADDRESS, CURSOR_LOCATION_HIGH);
	outb(PORT_CRTC_DATA, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t get_cursor_pos(void)
{
	uint16_t pos = 0;
	outb(PORT_CRTC_ADDRESS, CURSOR_LOCATION_LOW);
	pos |= inb(PORT_CRTC_DATA);
	outb(PORT_CRTC_ADDRESS, CURSOR_LOCATION_HIGH);
	pos |= ((uint16_t)inb(PORT_CRTC_DATA)) << 8;
	return pos;
}

uint16_t get_cursor_x(const uint16_t pos)
{
	return pos % VGA_WIDTH;
}

uint16_t get_cursor_y(const uint16_t pos)
{
	return pos / VGA_WIDTH;
}
