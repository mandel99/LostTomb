#include "placement_preview.h"
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This canonical entry requires the original MSVC i386 member ABI.
#endif
/* Native ECX receiver is explicit at the unchanged C algorithm boundary.
   Logical result is void. Volatile registers and flags are not C outputs. */
__declspec(naked) void FUN_004e5490(void)
{
 __asm {
  push dword ptr [esp+8]
  push dword ptr [esp+8]
  push ecx
  call restore_placement_member_004e5490
  add esp,12
  ret 8
 }
}
