#include "crt_runtime_init.h"

uint32_t restore_crt_control87(uint32_t value,uint32_t mask,volatile uint32_t *native_mask_slot)
{
 uint16_t control=0;
 uint32_t requested,encoded;
 /* Native FSTCW waits for pending unmasked x87 exceptions. */
 __asm fstcw control
 /* Existing decoder reads only low13 bits; native DWORD padding is dead. */
 requested=FUN_0056880a((uint32_t)control);
 requested=(requested&~mask)|(value&mask);
 encoded=FUN_0056889c(requested);
 if(native_mask_slot){
  *native_mask_slot=encoded;
  control=*(volatile uint16_t *)native_mask_slot;
 }else control=(uint16_t)encoded;
 __asm fldcw control
 return requested;
}

/* Preserve the original write to the incoming mask DWORD as well as RET0.
   Entry EAX is dead; only the two explicit arguments are live. */
#pragma warning(suppress:4100)
__declspec(naked) uint32_t FUN_005687bf(uint32_t value,uint32_t mask)
{
 __asm {
  lea eax,[esp+8]
  push eax
  push dword ptr[esp+12]
  push dword ptr[esp+12]
  call restore_crt_control87
  add esp,12
  ret
 }
}
