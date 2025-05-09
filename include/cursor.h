#ifndef KFS_CURSOR_H
#define KFS_CURSOR_H

#include <stddef.h>
#include <stdint.h>
#include "screen.h"

void move_cursor(const int x, const int y);

uint16_t get_cursor_pos(void);

uint16_t get_cursor_x(const uint16_t pos);

uint16_t get_cursor_y(const uint16_t pos);

#endif /* KFS_CURSOR_H */
