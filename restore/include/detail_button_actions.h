#ifndef RESTORE_DETAIL_BUTTON_ACTIONS_H
#define RESTORE_DETAIL_BUTTON_ACTIONS_H
#include <stdint.h>
/* Three whole nullary cdecl/RET0 callbacks, without incoming ECX receiver.
   Native return registers are not consumed by the button dispatcher. */
void FUN_004071c0(void);
void FUN_00407290(void);
void FUN_00407320(void);
int restore_detail_button_action(uint32_t token);
#endif
