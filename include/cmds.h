#ifndef KFS_CMDS_H
#define KFS_CMDS_H

#include "utils.h"
#include "screen.h"
#include "date.h"

void cmd_help(void);

void cmd_exit(void);

void cmd_date(void);

void parse_commands(const char *input);

#endif /* KFS_CMDS_H */
