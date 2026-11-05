#include "crt_exception_runtime.h"

/* NLG uses EAX, ECX and EBP, preserves flags/registers, and consumes one DWORD.
   Its argument is already in ECX; the stack copy is deliberately not read. */
__declspec(naked) void FUN_00561875(void)
{
 __asm {
  pushfd
  pushad
  mov eax,[esp+28]
  mov ecx,[esp+24]
  mov edx,[esp+8]
  push edx
  push ecx
  push eax
  call restore_crt_notify_jump
  add esp,12
  popad
  popfd
  ret 4
 }
}

/* Keep the native RET12 entry separate from the ordinary C API. */
__declspec(naked) void FUN_005640a0(void)
{
 __asm {
  push [esp+12]
  push [esp+12]
  push [esp+12]
  call restore_crt_invoke_frame
  add esp,12
  ret 12
 }
}

__declspec(noreturn) void FUN_005637ee(void)
{
 restore_crt_terminate();
}

/* Windows invokes the filter with stdcall cleanup; retain the C lookup name. */
__declspec(naked) void FUN_0056870b(void)
{
 __asm {
  push [esp+4]
  call restore_crt_unhandled_filter
  add esp,4
  ret 4
 }
}
