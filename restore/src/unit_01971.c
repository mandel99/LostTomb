#include "general_placement.h"
__declspec(naked) uint32_t FUN_004e5790(void)
{
 __asm {
  push ecx
  push eax
  push dword ptr[esp+20]
  push dword ptr[esp+20]
  push dword ptr[esp+20]
  call restore_placement_existing_004e5790
  add esp,16
  pop ecx
  ret 12
 }
}
