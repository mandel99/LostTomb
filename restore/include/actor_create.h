#ifndef RESTORE_ACTOR_CREATE_H
#define RESTORE_ACTOR_CREATE_H
#include <stdint.h>
int32_t restore_actor_create(uint8_t player,int32_t kind,int32_t x,int32_t y,uint8_t direction);
int32_t FUN_004b4800(uint8_t player,int32_t kind,int32_t x,int32_t y,uint8_t direction);
uint8_t FUN_004b47a0(int32_t actor);
uint32_t restore_ring_004f6570(const void *queue,int32_t actor);
void FUN_00444b30(int32_t actor);
#endif
