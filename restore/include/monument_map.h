#ifndef RESTORE_MONUMENT_MAP_H
#define RESTORE_MONUMENT_MAP_H
#include "building_methods.h"
/* ECX receiver + building DWORD / native RET4; public C has explicit self.
   Queries use the genuine image member and its flags, not incoming BL. */
void restore_monument_map_004017a0(uint32_t self,uint32_t building);
void restore_monument_map_0040dca0(uint32_t self,uint32_t building);
void restore_monument_map_004e69c0(uint32_t self,uint32_t building);
void restore_monument_map_frame(uint32_t target,struct restore_menu_input_frame *frame);
#define RESTORE_MONUMENT_MAP_ENTRY(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push dword ptr[esp+4] } __asm { push ecx } \
 __asm { call body } __asm { add esp,8 } __asm { ret } }
#endif
