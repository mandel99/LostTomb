#ifndef RESTORE_MONUMENT_ENTRANCE_H
#define RESTORE_MONUMENT_ENTRANCE_H
#include <stdint.h>
/* Original position members define AL=1 and retain meaningful high EAX.
   Outputs are ordered DWORD stores to physical 32-bit addresses. */
uint32_t restore_monument_entrance_0040eb90(uint32_t self,uint32_t building,uint32_t out_x,uint32_t out_y);
uint32_t restore_monument_entrance_00417490(uint32_t building,uint32_t out_x,uint32_t out_y);
uint32_t restore_monument_entrance_00401af0(uint32_t building,uint32_t out_x,uint32_t out_y);
uint32_t restore_monument_entrance_004e6ae0(uint32_t building,uint32_t out_x,uint32_t out_y);
uint32_t restore_monument_entrance_query(uint32_t target,uint32_t self,uint32_t building,uint32_t out_x,uint32_t out_y);
/* Native 40EB90: ECX receiver + three stack arguments, RET12.
   The legacy C alias captures ECX and uses standard caller cleanup. */
#define RESTORE_MONUMENT_ENTRANCE_ENTRY(name) \
 __declspec(naked) void name(void) { \
 __asm { push dword ptr[esp+12] } __asm { push dword ptr[esp+12] } \
 __asm { push dword ptr[esp+12] } __asm { push ecx } \
 __asm { call restore_monument_entrance_0040eb90 } __asm { add esp,16 } __asm { ret } }
#endif
