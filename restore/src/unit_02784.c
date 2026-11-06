#include "crt_translation_guard.h"

/* FS:[0] targets are listed in crt_exception_handlers.asm SafeSEH metadata. */
#pragma warning(push)
#pragma warning(disable:4733)

static void restore_crt_finish_exception_unwind(EXCEPTION_RECORD *exception,
    uint32_t previous)
{
    exception->ExceptionFlags&=~2u;
    *(volatile uint32_t *)(uintptr_t)previous=__readfsdword(0);
    _ReadWriteBarrier();
    __writefsdword(0,previous);
    _ReadWriteBarrier();
}

#pragma warning(pop)

/* Only the OS continuation and suspended C stack are assembly. The label is
   in this image; native56152D must never remain an absolute code address. */
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) void restore_crt_unwind_exception(uint32_t target,
    EXCEPTION_RECORD *exception)
{
    __asm {
        push ebp
        mov ebp,esp
        push ebx
        push esi
        push edi
        mov eax,fs:[0]
        push eax
        push ebp
        push 0
        push [ebp+12]
        push offset exception_unwind_resumed
        push [ebp+8]
        call RtlUnwind
    exception_unwind_resumed:
        pop ebp
        pop edx
        push edx
        push [ebp+12]
        call restore_crt_finish_exception_unwind
        add esp,8
        pop edi
        pop esi
        pop ebx
        mov esp,ebp
        pop ebp
        ret
    }
}
#pragma warning(pop)

/* Existing C callers use the typed cdecl API above. Preserve native RET8
   separately, with the same undecorated canonical linker symbol. */
__declspec(naked) void FUN_00561505(void)
{
    __asm {
        push [esp+8]
        push [esp+8]
        call restore_crt_unwind_exception
        add esp,8
        ret 8
    }
}
