#include "crt_translation_guard.h"

/* FS:[0] targets are listed in crt_exception_handlers.asm SafeSEH metadata. */
#pragma warning(push)
#pragma warning(disable:4733)

typedef void (__cdecl *RestoreCrtSeTranslator)(uint32_t,EXCEPTION_POINTERS *);

static void restore_crt_prepare_translation(volatile RestoreCrtTranslationFrame *f,
    const uint32_t *arguments,uint32_t continuation,uint32_t saved_sp,
    uint32_t saved_bp)
{
    f->guard.previous=0;
    f->guard.handler=(uint32_t)(uintptr_t)FUN_005616b7;
    f->guard.descriptor=arguments[4];
    f->guard.frame=arguments[1];
    f->guard.depth=arguments[5];
    f->guard.registration=arguments[6];
    f->guard.continuation=0;
    f->guard.saved_sp=0;
    f->guard.saved_bp=0;
    f->guard.unwind_seen=0;
    f->guard.continuation=continuation;
    f->guard.saved_sp=saved_sp;
    f->guard.saved_bp=saved_bp;
    f->guard.previous=__readfsdword(0);
    _ReadWriteBarrier();
    __writefsdword(0,(uint32_t)(uintptr_t)&f->guard);
    _ReadWriteBarrier();
    f->translated=1;
    f->exception=arguments[0];
    f->context=arguments[2];
}

static void restore_crt_run_translation(volatile RestoreCrtTranslationFrame *f)
{
    RestoreCrtSeTranslator translator=*(RestoreCrtSeTranslator volatile *)0x01266410u;
    translator(restore_crt_eh_read32(f->exception),
        (EXCEPTION_POINTERS *)(uintptr_t)&f->exception);
    f->translated=0;
}

static uint32_t restore_crt_finish_translation(volatile RestoreCrtTranslationFrame *f)
{
    uint32_t previous=f->guard.previous;
    if(f->guard.unwind_seen!=0){
        uint32_t chain=__readfsdword(0);
        *(volatile uint32_t *)(uintptr_t)previous=restore_crt_eh_read32(chain);
    }
    _ReadWriteBarrier();
    __writefsdword(0,previous);
    _ReadWriteBarrier();
    return f->translated;
}

#pragma warning(pop)

/* Capture the real52-byte frame and a relocated continuation. Both normal
   return and handler-driven stack restoration finish in the same C owner. */
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) uint32_t restore_crt_translate_exception(uint32_t *exception,
    uint32_t frame,uint32_t context,uint32_t reserved,uint32_t descriptor,
    uint32_t depth,uint32_t registration)
{
    __asm {
        push ebp
        mov ebp,esp
        sub esp,52
        push ebx
        push esi
        push edi
        mov edx,esp
        push ebp
        push edx
        push offset translation_resumed
        lea ecx,[ebp+8]
        push ecx
        lea eax,[ebp-52]
        push eax
        call restore_crt_prepare_translation
        add esp,20
        lea eax,[ebp-52]
        push eax
        call restore_crt_run_translation
        add esp,4
    translation_resumed:
        lea eax,[ebp-52]
        push eax
        call restore_crt_finish_translation
        add esp,4
        pop edi
        pop esi
        pop ebx
        mov esp,ebp
        pop ebp
        ret
    }
}
__declspec(naked) uint32_t FUN_00561603(uint32_t *exception,uint32_t frame,
    uint32_t context,uint32_t reserved,uint32_t descriptor,uint32_t depth,
    uint32_t registration)
{
    __asm jmp restore_crt_translate_exception
}
#pragma warning(pop)
