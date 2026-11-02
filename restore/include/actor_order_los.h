#ifndef RESTORE_ACTOR_ORDER_LOS_H
#define RESTORE_ACTOR_ORDER_LOS_H
#include <stdint.h>
void restore_los_slopes80(int32_t dx,int32_t dy,uint8_t slope[10],uint8_t inverse[10]);
void restore_los_intersection80(int32_t origin,int32_t delta,const uint8_t ratio[10],uint8_t result[10]);
uint32_t restore_los_equal_max80(const uint8_t value[10]);
uint64_t restore_los_trunc80(const uint8_t value[10]);
int32_t FUN_0040bda0(int32_t x,int32_t y,int32_t tx,int32_t ty,int32_t *out_x,int32_t *out_y);
int32_t FUN_0040c180(int32_t x,int32_t y,int32_t tx,int32_t ty,int32_t *out_x,int32_t *out_y);
#endif
