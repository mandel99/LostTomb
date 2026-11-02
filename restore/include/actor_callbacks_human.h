#ifndef RESTORE_ACTOR_CALLBACKS_HUMAN_H
#define RESTORE_ACTOR_CALLBACKS_HUMAN_H
#include "restore_types.h"
#include <stdint.h>
void restore_actor_00489ca0(void);
void restore_actor_0048a260(void);
uint8_t restore_actor_human_boundary(int32_t x,int32_t unused_y);
uint32_t FUN_0047a1c0(int32_t x);
void FUN_004b17d0(int32_t id),FUN_004b1d50(int32_t id),FUN_004b3250(int32_t id);
void FUN_004a5da0(int32_t id,uint32_t speed);
uint32_t FUN_0047fe60(int32_t x,int32_t y,uint32_t kind,int32_t distance);
void FUN_004b4220(int32_t id,int32_t from_x,int32_t from_y,int32_t to_x,int32_t to_y,int32_t mode);
int32_t FUN_004b3ff0(int32_t id,int32_t mode);
void FUN_0046cea0(int32_t building,int16_t kind),FUN_004e90f0(int32_t player,int32_t amount);
#endif
