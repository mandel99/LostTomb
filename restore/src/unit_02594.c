#include "placement_context.h"
__declspec(naked) uint32_t FUN_0054f430(void)
{
 __asm {
  push ecx
  push eax
  push ecx
  call restore_placement_existing_0054f430
  add esp,8
  pop ecx
  ret
 }
}
