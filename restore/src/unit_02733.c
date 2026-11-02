#include "restore_types.h"
/* QuickTime's dispatcher receives its selector in EAX, while retaining the
 * original caller's argument stack. A normal C indirect call loses that ABI.
 * This six-instruction boundary contains no embedded original game code. */
__declspec(naked) uint32_t FUN_005605a0(uint32_t argument)
{
  __asm {
    mov eax,dword ptr ds:[00604804h]
    or eax,eax
    jz dispatcher_ready
    call eax
dispatcher_ready:
    mov eax,000200ddh
    jmp dword ptr ds:[00604808h]
  }
}
