#ifndef RESTORE_MENU_REMAINING_ACTIONS_H
#define RESTORE_MENU_REMAINING_ACTIONS_H
#include <stdint.h>
/* Nullary native cdecl/RET0 input callbacks. The button engines discard
   incidental volatile-register results. Port names are reference labels. */
int restore_menu_remaining_action(uint32_t token);
#endif
