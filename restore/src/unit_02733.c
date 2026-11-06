#include "restore_types.h"
#include "quicktime_dispatch.h"
/* QuickTime's dispatcher receives its selector in EAX, while retaining the
 * original caller's argument stack. A normal C indirect call loses that ABI.
 * This six-instruction boundary contains no embedded original game code. */
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) uint32_t FUN_005605a0(uint32_t argument)
{
  __asm {
    push ecx
    push edx
    call restore_quicktime_relocate_slots
    pop edx
    pop ecx
    mov eax,dword ptr ds:[00604804h]
    or eax,eax
    jz dispatcher_ready
    call eax
dispatcher_ready:
    mov eax,000200ddh
    jmp dword ptr ds:[00604808h]
  }
}

#pragma warning(pop)
