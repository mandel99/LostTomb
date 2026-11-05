#include "placement_context.h"
__declspec(naked) uint32_t FUN_0040d880(void)
{
 __asm {
  push ecx
  push eax
  push ecx
  call restore_placement_existing_0040d880
  add esp,8
  pop ecx
  ret
 }
}
