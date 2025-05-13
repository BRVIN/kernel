#ifndef KFS_KERNEL_H
#define KFS_KERNEL_H

#if defined(__linux__)
#error "linux undefined"
#endif

#if !defined(__i386__)
#error "no ix86-elf compiler"
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"
#include "io.h"
#include "screen.h"
#include "keyboard.h"
#include "draw.h"
#include "cursor.h"


#endif /* KFS_KERNEL_H */
