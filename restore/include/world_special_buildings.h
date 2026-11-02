#ifndef RESTORE_WORLD_SPECIAL_BUILDINGS_H
#define RESTORE_WORLD_SPECIAL_BUILDINGS_H
#include <stdint.h>
/* Typed C boundaries are cdecl. The Abu Simbel adapter normalizes the
   original RET12; the other complete native bodies return with RET0.
   Annex4C0160 consumes only the building, despite two extra pushes at its
   city-render call site. Its coordinates come from real map projection. */
void FUN_00401f70(int32_t building,int32_t x,int32_t y);
void FUN_004c0160(int32_t building);
void FUN_004ef550(uint32_t image,int32_t x,int32_t y,int32_t building);
void FUN_00542e10(int32_t building,int32_t x,int32_t y);
#endif
