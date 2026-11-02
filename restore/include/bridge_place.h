#ifndef RESTORE_BRIDGE_PLACE_H
#define RESTORE_BRIDGE_PLACE_H
#include <stdint.h>
/* Explicit raw-frame API: native_args is the original building,x,y argument
   array, whose first DWORD is an in/out image slot. incoming_ecx must be the
   caller's saved frame ECX, not a compiler-selected physical register.
   Return is imageB, which the native final POP publishes in ECX. */
uint32_t restore_bridge_00440540(uint32_t native_args[3],uint32_t incoming_ecx);
/* Physical x86 entry: cdecl3/RET0, captures incoming ECX before a C prologue.
   Its incidental EAX/EDX/flags and private stack scratch are not public results. */
void FUN_00440540(int32_t building,int32_t x,int32_t y);
#endif
