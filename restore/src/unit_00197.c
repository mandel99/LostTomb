#include "placement_preview.h"
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This canonical entry requires the original MSVC i386 member ABI.
#endif
/* Native ECX receiver is explicit at the unchanged C algorithm boundary.
   Logical result is void. Volatile registers and flags are not C outputs. */
void restore_placement_draw_00416f90(uint32_t self)
{
 restore_placement_member_004e5490((void *)(uintptr_t)self,
  (const void *)(uintptr_t)(self+0x4cu),0x9);
}
__declspec(naked) void FUN_00416f90(void)
{
 __asm {
  push ecx
  call restore_placement_draw_00416f90
  add esp,4
  ret
 }
}
