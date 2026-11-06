#include "quicktime_dispatch.h"
/* ABI-only gate: EAX selector, original argument stack and tail result.
   Untyped raw gates retain their caller stack; no argument values are invented. */
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) uint32_t FUN_005605e0(uint32_t controller,const RestoreQuickTimeComponentParameters *parameters)
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
        call dword ptr ds:[00604804h]
dispatcher_ready:
        mov eax,65536
        jmp dword ptr ds:[00604808h]
    }
}
#pragma warning(pop)
