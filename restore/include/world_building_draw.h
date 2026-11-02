#ifndef RESTORE_WORLD_BUILDING_DRAW_H
#define RESTORE_WORLD_BUILDING_DRAW_H
#include <stdint.h>
/* Logical void C facade for native thiscall488C20, four DWORDs/RET16.
 * self is original ECX; saved_ebx is the original incoming EBX, whose saved
 * slot is read by the native default EBP=-1 lookup. Neither is inferred
 * from compiler-owned physical registers. Existing raw-frame callers should
 * keep restore_building_refresh_raw when they need the complete register API.
 * Numeric/renderer qualifications of building_refresh_helpers still apply. */
void restore_building_draw_00488c20(uint32_t self,uint32_t saved_ebx,int32_t building,int32_t x,int32_t y,uint32_t mode);
/* Exact435FDE/4371D5 call contexts both PUSH EBX as the building argument;
 * incoming ECX is BCA350. This wrapper faithfully binds saved_ebx=building. */
void restore_world_building_draw_00488c20(uint32_t self,int32_t building,int32_t x,int32_t y,uint32_t mode);
#endif
