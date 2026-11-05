#include <stdint.h>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This entry requires the original i386 ABI.
#endif
/* Complete native template. Arithmetic/address wrap is explicitly32-bit.
   Canonical volatile outputs and final flags are reconstructed separately.
   Nonfaulting ordinary-memory inputs only; no whole-game equivalence claim. */
uint32_t restore_object_find_key_0041b9d0(uint32_t self,uint32_t key,int32_t *native_count)
{
 int32_t count=*(volatile int32_t *)(uintptr_t)(self+0x00001900u);
 uint32_t index;
 if(native_count)*native_count=count;
 for(index=0;count>0&&index<(uint32_t)count;++index)
  if(*(volatile uint32_t *)(uintptr_t)(self+0x0000000cu+index*64u)==key)return index;
 return 0xffffffffu;
}
__declspec(naked) void FUN_0041b9d0(void)
{
 __asm {
  push ebp
  mov ebp,esp
  sub esp,8
  mov [ebp-8],ecx
  lea eax,[ebp-4]
  push eax
  push dword ptr[ebp+8]
  push ecx
  call restore_object_find_key_0041b9d0
  add esp,12
  mov ecx,[ebp-8]
  mov edx,[ebp-4]
  mov esp,ebp
  pop ebp
  test edx,edx
  jle failed
  add ecx,0ch
  cmp eax,-1
  je exhausted
  push eax
  imul eax,64
  add ecx,eax
  pop eax
  cmp eax,eax
  ret 4
 exhausted:
  push eax
  mov eax,edx
  imul eax,64
  add ecx,eax
  pop eax
 failed:
  or eax,-1
  ret 4
 }
}
