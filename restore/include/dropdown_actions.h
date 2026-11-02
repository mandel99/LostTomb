#ifndef RESTORE_DROPDOWN_ACTIONS_H
#define RESTORE_DROPDOWN_ACTIONS_H
#include <stdint.h>
/* Native table callbacks: no arguments; the dropdown discards EAX. */
void FUN_00402bc0(void);
void FUN_00402be0(void);
void FUN_00402e30(void);
void FUN_00402ee0(void);
void FUN_004032c0(void);
void FUN_00406070(void);
void FUN_004060e0(void);
void FUN_00406100(void);
void FUN_00406140(void);
void FUN_00406170(void);
int restore_dropdown_action(uint32_t token);
#endif
