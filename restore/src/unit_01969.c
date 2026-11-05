#include "general_placement.h"
/* Existing typed C query is reused unchanged. This is only ECX/RET adaptation.
   Full EAX is forwarded; legacy cdecl callers migrate as one closed group. */
__declspec(naked) uint32_t FUN_004e5330(void)
{
 __asm {
  push dword ptr[esp+12]
  push dword ptr[esp+12]
  push dword ptr[esp+12]
  push ecx
  call restore_placement_check_004e5330
  add esp,16
  ret 12
 }
}
