#include "placement_context.h"
/* Existing typed C query is reused unchanged. This is only ECX/RET adaptation.
   Full EAX is forwarded; legacy cdecl callers migrate as one closed group. */
__declspec(naked) uint32_t FUN_0040d850(void)
{
 __asm {
  push dword ptr[esp+4]
  push ecx
  call restore_placement_check_0040d850
  add esp,8
  ret 4
 }
}
