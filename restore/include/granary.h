#ifndef RESTORE_GRANARY_H
#define RESTORE_GRANARY_H
#include <stdint.h>
int32_t restore_granary_gather_destination(int32_t actor,int32_t resource,uint32_t network);
int32_t restore_granary_find_gather_source(int32_t building);
void restore_granary_rebuild_supply(void);
int32_t restore_granary_take_gathered(int32_t destination,int32_t source);
int32_t restore_granary_deliver(int32_t building,int32_t resource,int32_t produced,int32_t amount);
int32_t restore_granary_withdraw(int32_t player,int32_t building,int32_t resource,int32_t amount);
int32_t FUN_0045dcc0(int32_t actor,int32_t resource,uint32_t network);
int32_t FUN_0045de80(int32_t building);
int32_t FUN_0045e150(int32_t destination,int32_t source);
int32_t FUN_0045e220(int32_t building,int32_t resource,int32_t produced,int32_t amount);
#endif
