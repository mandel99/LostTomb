#include "restore_types.h"

/* Whole native 56133C..561362, 15 instructions. The input is physically
   ST0, rather than an uninitialized C local. Preserve the caller's x87
   control word and return the signed integer in EDX:EAX. */
__declspec(naked) longlong FUN_0056133c(void)
{
 __asm {
  push ebp
  mov ebp,esp
  add esp,-12
  fstcw word ptr[ebp-2]
  fwait
  mov ax,word ptr[ebp-2]
  or ah,0ch
  mov word ptr[ebp-4],ax
  fldcw word ptr[ebp-4]
  fistp qword ptr[ebp-12]
  fldcw word ptr[ebp-2]
  mov eax,dword ptr[ebp-12]
  mov edx,dword ptr[ebp-8]
  leave
  ret
 }
}
