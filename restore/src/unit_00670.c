#include <stdint.h>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This entry requires the original i386 ABI.
#endif
/* Complete native template. Arithmetic/address wrap is explicitly32-bit.
   Canonical volatile outputs and final flags are reconstructed separately.
   Nonfaulting ordinary-memory inputs only; no whole-game equivalence claim. */
uint32_t restore_object_scale_clamp_004477d0(uint32_t self,int32_t *native_quotient,uint32_t *native_sign)
{
 uint32_t factor=*(volatile uint8_t *)(uintptr_t)(self+0x00000000u);
 uint32_t amount=*(volatile uint32_t *)(uintptr_t)(self+0x00000134u);
 int32_t product=(int32_t)(factor*amount);
 int32_t quotient=product/400;
 if(native_quotient)*native_quotient=quotient;
 if(native_sign)*native_sign=product<0?1u:0u;
 return quotient<1?1u:(uint32_t)quotient;
}
__declspec(naked) void FUN_004477d0(void)
{
 __asm {
  push ebp
  mov ebp,esp
  sub esp,8
  lea eax,[ebp-8]
  lea edx,[ebp-4]
  push edx
  push eax
  push ecx
  call restore_object_scale_clamp_004477d0
  add esp,12
  mov edx,[ebp-8]
  mov ecx,[ebp-4]
  mov esp,ebp
  pop ebp
  cmp edx,1
  ret
 }
}
