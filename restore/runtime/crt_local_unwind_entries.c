#include "crt_local_unwind.h"

__declspec(naked) void FUN_005617a8(void)
{
 __asm jmp restore_crt_global_unwind
}

uint32_t FUN_005617c8(EXCEPTION_RECORD *exception,uint32_t registration,
 void *context,uint32_t *dispatcher)
{
 return restore_crt_unwind_guard(exception,registration,context,dispatcher);
}

/* Preserve the actual incoming EBP as the implicit funclet environment. */
__declspec(naked) void FUN_005617ea(void)
{
 __asm {
  push ebp
  push [esp+12]
  push [esp+12]
  call restore_crt_local_unwind
  add esp,12
  ret
 }
}

int32_t FUN_00561852(void)
{
 return restore_crt_abnormal_termination();
}

__declspec(naked) void FUN_0056187e(void)
{
 __asm {
  pushfd
  pushad
  mov eax,[esp+28]
  mov edx,[esp+8]
  push edx
  push eax
  call restore_crt_notify_frame
  add esp,8
  popad
  popfd
  ret 4
 }
}

/* Native scope callbacks receive EBP and the observed EAX/ECX values.
   The indirect target is reloaded separately from the notification target. */
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) void restore_crt_run_scope(uint32_t target,uint32_t parent_frame,
 uint32_t entry_eax,uint32_t entry_ecx)
{
 __asm {
  push ebp
  push ebx
  push esi
  push edi
  mov ebp,[esp+24]
  mov eax,[esp+28]
  mov ecx,[esp+32]
  call dword ptr [esp+20]
  pop edi
  pop esi
  pop ebx
  pop ebp
  ret
 }
}
#pragma warning(pop)
