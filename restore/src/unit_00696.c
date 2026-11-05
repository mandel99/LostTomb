#include <stdint.h>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This entry preserves the original MSVC i386 register ABI.
#endif
/* Generated from a complete native MOV/LEA rule; nonfaulting accesses.
   Logical result, access width and canonical register/flag preservation
   are explicit. Whole-game/exception-context equivalence is unproved. */
uint32_t restore_object_lookup_u8_004493d0(uint32_t self)
{
 uint32_t index=*(volatile uint8_t *)(uintptr_t)(self+0x00000002u);
 return (uint32_t)*(volatile uint32_t *)(uintptr_t)(0x005d9c98u+index*4u);
}
__declspec(naked) void FUN_004493d0(void)
{
 __asm {
  pushfd
  push ecx
  push edx
  push ecx
  call restore_object_lookup_u8_004493d0
  add esp,4
  pop edx
  pop ecx
  popfd
  push eax
  xor eax,eax
  pop eax
  ret
 }
}
