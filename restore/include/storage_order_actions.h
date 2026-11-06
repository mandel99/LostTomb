#ifndef RESTORE_STORAGE_ORDER_ACTIONS_H
#define RESTORE_STORAGE_ORDER_ACTIONS_H
#include <stdint.h>
/* Native button callback: actual5C68C8 records publish one-based ordinals.
   The original parent computes their count with the same resource filter. */
int restore_storage_order_action(uint32_t token);
void restore_storage_adjust_limit_00406460(void);
void FUN_00406460(void);
#endif
