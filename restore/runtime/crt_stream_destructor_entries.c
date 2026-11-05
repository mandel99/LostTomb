#include "crt_stream_destructor.h"

/* Canonical entries adapt only ECX/stack or the original parent EBP.
   All decisions, object operations and cleanup phases remain in C. */

__declspec(naked) uint32_t FUN_0056062c(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_ios_scalar_delete
        add esp, 8
        ret 4
    }
}

__declspec(naked) uint32_t FUN_005606c6(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_filebuf_scalar_delete
        add esp, 8
        ret 4
    }
}

__declspec(naked) uint32_t FUN_00560f2f(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_buffer_scalar_delete
        add esp, 8
        ret 4
    }
}

__declspec(naked) void FUN_005606e2(void)
{
    __asm {
        push ecx
        call restore_stream_filebuf_destroy
        add esp, 4
        ret
    }
}

__declspec(naked) void FUN_00560f4b(void)
{
    __asm {
        push ecx
        call restore_stream_buffer_destroy
        add esp, 4
        ret
    }
}

__declspec(naked) uint32_t FUN_00561090(void)
{
    __asm {
        push ecx
        call restore_stream_buffer_pending
        add esp, 4
        ret
    }
}

__declspec(naked) void FUN_00560d87(void)
{
    __asm {
        push ecx
        call restore_stream_ostream_destroy
        add esp, 4
        ret
    }
}

__declspec(naked) void FUN_005612ed(void)
{
    __asm {
        push ecx
        call restore_stream_iostream_destroy
        add esp, 4
        ret
    }
}

__declspec(naked) void FUN_00560a9b(void)
{
    __asm {
        push ecx
        call restore_stream_fstream_destroy
        add esp, 4
        ret
    }
}

__declspec(naked) void FUN_0053dfd0(void)
{
    __asm {
        push ecx
        call restore_stream_fstream_complete_destroy
        add esp, 4
        ret
    }
}

__declspec(naked) void FUN_0056b7d0(void)
{
    __asm {
        push ebp
        call restore_stream_cleanup_56b7d0
        add esp, 4
        ret
    }
}

__declspec(naked) void FUN_0056e17c(void)
{
    __asm {
        push ebp
        call restore_stream_cleanup_56e17c
        add esp, 4
        ret
    }
}

__declspec(naked) void FUN_0056e190(void)
{
    __asm {
        push ebp
        call restore_stream_cleanup_56e190
        add esp, 4
        ret
    }
}

__declspec(naked) void FUN_0056e1c0(void)
{
    __asm {
        push ebp
        call restore_stream_cleanup_56e1c0
        add esp, 4
        ret
    }
}

__declspec(naked) void FUN_0056e1e4(void)
{
    __asm {
        push ebp
        call restore_stream_cleanup_56e1e4
        add esp, 4
        ret
    }
}

__declspec(naked) void FUN_0056e208(void)
{
    __asm {
        push ebp
        call restore_stream_cleanup_56e208
        add esp, 4
        ret
    }
}

__declspec(naked) void FUN_0056e222(void)
{
    __asm {
        push ebp
        call restore_stream_cleanup_56e222
        add esp, 4
        ret
    }
}

__declspec(naked) void FUN_0056e238(void)
{
    __asm {
        push ebp
        call restore_stream_cleanup_56e238
        add esp, 4
        ret
    }
}

__declspec(naked) uint32_t FUN_00560a70(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_fstream_scalar_delete
        add esp, 8
        ret 4
    }
}

__declspec(naked) uint32_t FUN_00560cf7(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_ostream_scalar_delete
        add esp, 8
        ret 4
    }
}

__declspec(naked) uint32_t FUN_0056119d(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_istream_scalar_delete
        add esp, 8
        ret 4
    }
}

__declspec(naked) uint32_t FUN_0056124d(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_iostream_scalar_delete
        add esp, 8
        ret 4
    }
}

#pragma warning(push)
#pragma warning(disable:4100) /* Naked edge reads only documented stack slots. */
__declspec(naked) void restore_stream_release_call(uint32_t target, uint32_t buffer)
{
    __asm {
        mov eax, dword ptr [esp + 4]
        mov ecx, dword ptr [esp + 8]
        push ecx
        call eax
        add esp, 4
        ret
    }
}
#pragma warning(pop)
