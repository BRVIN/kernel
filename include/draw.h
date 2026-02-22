#ifndef KFS_DRAW_H
#define KFS_DRAW_H

#include "screen.h"

void draw_guides();
void draw_42_logo();
void draw_reset_top_bar();
void draw_tabs();
void draw_dbg_scancode(const uint8_t scancode);
void draw_dbg_cursor_pos(void);
void draw_dbg_input(char *str);

#endif /* KFS_DRAW_H */
