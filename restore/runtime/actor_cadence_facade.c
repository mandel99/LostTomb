#include "actor_cadence_closure.h"
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This native cdecl facade requires the MSVC i386 ABI.
#endif

/* Canonical two-argument ABI: capture actual entry EAX before any C
   prologue. Third argument is pushed first. After each push the original
   next argument is at ESP+12. C helper preserves EBX/ESI/EDI/EBP.
   Native 4A3B00 is RET0; the original caller retains its two arguments. */
#pragma warning(suppress:4100)
__declspec(naked) int32_t FUN_004a3b00(int32_t actor,int32_t period)
{
 __asm {
  push eax
  push dword ptr [esp+12]
  push dword ptr [esp+12]
  call restore_actor_advance_cadence
  add esp,12
  ret
 }
}
