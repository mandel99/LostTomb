#include "placement_queries.h"
__declspec(naked) uint32_t FUN_00547ba0(void)
{
 __asm {
  push dword ptr[esp+4]
  push ecx
  call restore_placement_activity_00547ba0
  add esp,8
  ret 4
 }
}
