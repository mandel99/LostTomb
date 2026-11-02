#ifndef RESTORE_SIDEBAR_COMMAND_H
#define RESTORE_SIDEBAR_COMMAND_H
#include <stdint.h>
/* Whole nullary native403D70: cdecl/RET0, preserves EBX, no incoming ECX.
   Button dispatcher discards return registers. Both native callees are the
   real canonical typed implementations; no alternate/safe-subset dispatch. */
void restore_sidebar_command_00403d70(void);
int restore_sidebar_command_action(uint32_t token);
#endif
