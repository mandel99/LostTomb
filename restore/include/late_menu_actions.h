#ifndef RESTORE_LATE_MENU_ACTIONS_H
#define RESTORE_LATE_MENU_ACTIONS_H
#include <stdint.h>
/* Whole nullary cdecl/RET0 button callbacks. No incoming receiver/FPU input;
   the 28-byte button dispatcher does not consume their return registers. */
void restore_late_menu_0053d730(void);
void restore_late_menu_005598a0(void);
void restore_late_menu_00559a60(void);
void restore_late_menu_00559b50(void);
void restore_late_menu_00559d70(void);
void restore_late_menu_00559e00(void);
/* Whole237-instruction price provider: one DWORD/cdeclRET0, full EAX;
   FUN alias replaces unit02171 so existing genuine443330 uses this core.
   Synchronous actual helper calls: AL1 implies the worker slot stays nonzero,
   so complex summaries are fully written without initializing caller scratch.
   Concurrent external writes to those slots are outside this contract. */
int32_t restore_late_menu_004f57b0(int32_t resource);
int32_t FUN_004f57b0(int32_t resource);
int restore_late_menu_action(uint32_t token);
#endif
