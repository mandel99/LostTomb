#ifndef RESTORE_GOD_COMMAND_ACTIONS_H
#define RESTORE_GOD_COMMAND_ACTIONS_H
#include <stdint.h>
/* Native command tokens require the original decoder-derived thirty-byte
   prestate. Unrelated callers cannot manufacture it from a host C frame. */
int restore_god_command_action(uint32_t token);
#endif
