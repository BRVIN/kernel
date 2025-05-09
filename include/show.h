#ifndef KFS_SHOW_H
#define KFS_SHOW_H

#include "screen.h"
#include "cursor.h"

void show_guides();
void show_42();
void show_tabs();
void draw_dbg_scancode(const uint8_t scancode);
void draw_dbg_cursor_pos(void);

#endif /* KFS_SHOW_H */
