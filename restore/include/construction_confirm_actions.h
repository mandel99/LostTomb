#ifndef RESTORE_CONSTRUCTION_CONFIRM_ACTIONS_H
#define RESTORE_CONSTRUCTION_CONFIRM_ACTIONS_H
#include <stdint.h>
/* Nullary cdecl/RET0 callbacks. A BYTE answer of exactly1 writes DWORD+1;
   every other BYTE value writes DWORD-1. Both outcomes rerun the genuine
   five-argument construction controller; cancellation is not an early RET.
   No incoming ECX or volatile return is consumed by the modal caller. */
void restore_construction_confirm_004021d0(void);
void restore_construction_confirm_00402210(void);
void restore_construction_confirm_00402250(void);
void restore_construction_confirm_00402290(void);
int restore_construction_confirm_action(uint32_t token);
#endif
