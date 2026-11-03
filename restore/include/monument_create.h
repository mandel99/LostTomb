#ifndef RESTORE_MONUMENT_CREATE_H
#define RESTORE_MONUMENT_CREATE_H
#include "building_methods.h"
/* Native members: ECX receiver, x/y/root/skip_registration, RET16.
   Public C entries use explicit receivers and ordinary caller cleanup. */
void restore_monument_create_0040da90(uint32_t self,int32_t x,int32_t y,uint32_t root,uint8_t skip_registration);
void restore_monument_create_004015a0(uint32_t self,int32_t x,int32_t y,uint32_t root,uint8_t skip_registration);
void restore_monument_create_00417180(uint32_t self,int32_t x,int32_t y,uint32_t root,uint8_t skip_registration);
void restore_monument_create_004e6840(uint32_t self,int32_t x,int32_t y,uint32_t root,uint8_t skip_registration);
void restore_monument_create_frame(uint32_t target,struct restore_menu_input_frame *frame);
#define RESTORE_MONUMENT_CREATE_ENTRY(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push dword ptr[esp+16] } __asm { push dword ptr[esp+16] } \
 __asm { push dword ptr[esp+16] } __asm { push dword ptr[esp+16] } \
 __asm { push ecx } __asm { call body } __asm { add esp,20 } __asm { ret } }
#endif
