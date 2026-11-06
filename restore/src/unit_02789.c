#include "crt_translation_guard.h"

#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) __declspec(noreturn) static void restore_crt_resume_translation(
    uint32_t registration)
{
    __asm {
        mov ebx,[esp+4]
        mov esp,[ebx+28]
        mov ebp,[ebx+32]
        jmp dword ptr [ebx+24]
    }
}
#pragma warning(pop)

uint32_t restore_crt_translation_guard_handler(uint32_t exception,
    uint32_t registration,uint32_t context)
{
    volatile RestoreCrtTranslationGuard *guard=
        (volatile RestoreCrtTranslationGuard *)(uintptr_t)registration;
    __asm cld
    if((restore_crt_eh_read32(exception+4u)&0x66u)!=0){
        guard->unwind_seen=1;
        return 1;
    }
    (void)FUN_0056389a((int32_t *)(uintptr_t)exception,guard->frame,context,0,
        (int32_t *)(uintptr_t)guard->descriptor,(int32_t)guard->depth,
        guard->registration,1);
    if(guard->unwind_seen==0)
        restore_crt_unwind_exception(registration,
            (EXCEPTION_RECORD *)(uintptr_t)exception);
    /* Native561721 is a JMP after restoring ESP/EBP, never a C CALL. The
       saved continuation defines its own EAX/flags before consuming them. */
    restore_crt_resume_translation(registration);
}
uint32_t FUN_005616b7(uint32_t exception,uint32_t registration,uint32_t context)
{
    return restore_crt_translation_guard_handler(exception,registration,context);
}
