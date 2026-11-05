#include "placement_preview.h"
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This canonical entry requires the original MSVC i386 member ABI.
#endif
/* Native ECX receiver is explicit at the unchanged C algorithm boundary.
   Logical result is void. Volatile registers and flags are not C outputs. */
void restore_placement_draw_0054f410(uint32_t self)
{
 restore_placement_member_00545fc0((void *)(uintptr_t)self,
  (const void *)(uintptr_t)(self+0x5c88u),0x201);
}
__declspec(naked) void FUN_0054f410(void)
{
 __asm {
  push ecx
  call restore_placement_draw_0054f410
  add esp,4
  ret
 }
}
