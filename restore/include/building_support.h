#ifndef RESTORE_BUILDING_CREATE_SUPPORT_H
#define RESTORE_BUILDING_CREATE_SUPPORT_H
#include <stdint.h>
uint32_t FUN_00405ba0(void);
int32_t FUN_0046efd0(uint32_t owner);
void FUN_00482580(int32_t x,int32_t y,int32_t size,int32_t radius,uint32_t mask);
void FUN_004f4a60(uint32_t building,int32_t slot);
int32_t FUN_004f4f80(int32_t building);
/* Original nullary thiscall/RET0; explicit self, standard C facade.
   Kept private alongside the accepted restore_ring_004f65xx methods. */
uint32_t restore_ring_004f6520(void *self);
void FUN_0046cea0(int32_t building,int16_t kind);
void FUN_0046cf40(int32_t building);
#endif
