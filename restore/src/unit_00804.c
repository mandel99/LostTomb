#include "housing_cycle.h"
/* Capture inherited native inputs before a C prologue can overwrite them.
   The algorithm itself lives in restore_house_consume_goods. */
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) void FUN_00454b40(int32_t building)
{
 __asm {
  mov eax,[esp-12]
  mov ecx,[esp-8]
  mov edx,[esp-4]
  sub esp,20
  mov [esp],ebx
  mov [esp+4],eax
  mov [esp+8],ecx
  mov [esp+12],edx
  lea eax,[esp+24]
  mov [esp+16],eax
  mov eax,[eax]
  mov edx,esp
  push edx
  push eax
  call restore_house_consume_goods
  add esp,28
  ret
 }
}
#pragma warning(pop)
