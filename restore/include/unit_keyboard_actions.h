#ifndef RESTORE_UNIT_KEYBOARD_ACTIONS_H
#define RESTORE_UNIT_KEYBOARD_ACTIONS_H
#include <stdint.h>
/* Native nullary cdecl/RET0 keyboard and table-button callbacks. */
void restore_unit_keyboard_00406c90(void);
void restore_unit_keyboard_00406de0(void);
int restore_unit_keyboard_action(uint32_t token);
/* Whole4BC670, genuine one-DWORD/cdeclRET0 helper. */
void restore_unit_actor_return_004bc670(uint32_t id);
#endif
