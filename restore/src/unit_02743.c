#include <stdint.h>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This entry requires the original i386 ABI.
#endif
/* Complete native template. Arithmetic/address wrap is explicitly32-bit.
   Canonical volatile outputs and final flags are reconstructed separately.
   Nonfaulting ordinary-memory inputs only; no whole-game equivalence claim. */
uint32_t restore_object_clear_read_00560734(uint32_t self)
{
 *(volatile uint32_t *)(uintptr_t)(self+0x00000030u)&=0u;
 return *(volatile uint32_t *)(uintptr_t)(self+0x00000010u);
}
__declspec(naked) void FUN_00560734(void)
{
 __asm {
  push ecx
  push edx
  push ecx
  call restore_object_clear_read_00560734
  add esp,4
  pop edx
  pop ecx
  push 0
  and dword ptr[esp],0
  lea esp,[esp+4]
  ret
 }
}
