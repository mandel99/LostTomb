#ifndef RESTORE_INVASION_COMMANDS_H
#define RESTORE_INVASION_COMMANDS_H
#include <stdint.h>
/* Native query defines AL only; the typed C boundary returns Boolean32. */
uint32_t FUN_004467a0(int32_t *x,int32_t *y);
/* Logical native cdecl5/RET0, full EAX result, no incoming ECX/FPU input. */
uint32_t restore_invasion_00446a30(int32_t nation,int32_t count,int32_t entry,
                                  uint32_t argument3,int32_t event_index);
void restore_invasion_command_00446530(void);
void restore_invasion_command_004465a0(void);
int restore_invasion_command_action(uint32_t token);
/* In-place native frame bridge; preserves the caller's actual stack. */
void restore_invasion_raw(uint32_t target,uint32_t registers[8],uint32_t *flags);
#endif
