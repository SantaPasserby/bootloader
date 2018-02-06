#ifndef __CMD_BOOT_H
#define __CMD_BOOT_H
#include "vshell.h"
int do_boot(cmd_tbl_t * a, int b, int c , char* d[]);
int get_loadaddr(const char * str);
#endif
