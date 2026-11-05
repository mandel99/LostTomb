#include "placement_preview.h"
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This canonical entry requires the original MSVC i386 member ABI.
#endif
/* Native ECX receiver is explicit at the unchanged C algorithm boundary.
   Logical result is void. Volatile registers and flags are not C outputs. */
void restore_placement_draw_00555e20(uint32_t self)
{
 restore_placement_member_00545fc0((void *)(uintptr_t)self,
  (const void *)(uintptr_t)(self+0x4ee0u),0x1b5);
}
__declspec(naked) void FUN_00555e20(void)
{
 __asm {
  push ecx
  call restore_placement_draw_00555e20
  add esp,4
  ret
 }
}
