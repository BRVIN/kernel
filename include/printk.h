#ifndef KFS_PRINTK_H
#define KFS_PRINTK_H

#include <stdarg.h>
#include <stdint.h>

#define KERN_INFO "<6>" // Niveau d'information
#define KERN_ERR "<3>"  // Niveau d'erreur

int printk(const char *fmt, ...);

#endif /* KFS_PRINTK_H */