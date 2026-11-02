#ifndef RESTORE_REMAINING_RESOURCE_ACTIONS_H
#define RESTORE_REMAINING_RESOURCE_ACTIONS_H
#include <stdint.h>
/* Whole nullary cdecl/RET0 callbacks, no incoming receiver/FPU inputs.
   Both square and28-byte dispatchers discard their incidental return GPRs. */
void restore_resource_action_004074e0(void);
void restore_resource_action_0040ce60(void);
void restore_resource_action_0040ce70(void);
void restore_resource_action_0040cef0(void);
int restore_remaining_resource_action(uint32_t token);
#endif
