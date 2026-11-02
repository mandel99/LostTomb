#ifndef RESTORE_MENU_MESSAGE_ACTIONS_H
#define RESTORE_MENU_MESSAGE_ACTIONS_H
#include <stdint.h>
/* Native nullary cdecl/RET0 callbacks. No volatile result is consumed by
   the original button engines. No new message-index guards are introduced. */
int restore_menu_message_action(uint32_t token);
#endif
