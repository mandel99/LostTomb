#include "crt_stream_constructor.h"

uint32_t restore_stream_io_resolve(uint32_t token);

/* Only canonical ECX/stack adaptation lives here. All object operations,
   state transitions and cleanup decisions are in the C source owners. */
__declspec(naked) uint32_t FUN_005605fa(void)
{
    __asm {
        push ecx
        call restore_stream_ios_construct
        add esp, 4
        ret
    }
}

__declspec(naked) void FUN_00560648(void)
{
    __asm {
        push ecx
        call restore_stream_ios_destroy
        add esp, 4
        ret
    }
}

__declspec(naked) uint32_t FUN_00560671(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_ios_initialize
        add esp, 8
        ret 4
    }
}

__declspec(naked) uint32_t FUN_005606a0(void)
{
    __asm {
        push ecx
        call restore_stream_filebuf_construct
        add esp, 4
        ret
    }
}

__declspec(naked) uint32_t FUN_00560f02(void)
{
    __asm {
        push ecx
        call restore_stream_buffer_construct
        add esp, 4
        ret
    }
}

__declspec(naked) uint32_t FUN_005609da(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_fstream_construct
        add esp, 8
        ret 4
    }
}

__declspec(naked) uint32_t FUN_00560d22(void)
{
    __asm {
        push dword ptr [esp + 8]
        push dword ptr [esp + 8]
        push ecx
        call restore_stream_ostream_construct
        add esp, 12
        ret 8
    }
}

__declspec(naked) uint32_t FUN_005611c8(void)
{
    __asm {
        push dword ptr [esp + 8]
        push dword ptr [esp + 8]
        push ecx
        call restore_stream_istream_construct
        add esp, 12
        ret 8
    }
}

__declspec(naked) void FUN_0056123e(void)
{
    __asm {
        push ecx
        call restore_stream_istream_destroy
        add esp, 4
        ret
    }
}

__declspec(naked) uint32_t FUN_00561278(void)
{
    __asm {
        push dword ptr [esp + 8]
        push dword ptr [esp + 8]
        push ecx
        call restore_stream_iostream_construct
        add esp, 12
        ret 8
    }
}

#pragma warning(push)
#pragma warning(disable:4100) /* Naked edge reads its documented stack slots. */
__declspec(naked) void restore_stream_virtual_delete(uint32_t buffer,
    uint32_t flags)
{
    __asm {
        mov ecx, dword ptr [esp + 4]
        mov eax, dword ptr [ecx]
        push eax
        push ecx
        push dword ptr [eax]
        call restore_stream_io_resolve
        add esp, 4
        mov edx, eax
        pop ecx
        pop eax
        push dword ptr [esp + 8]
        call edx
        ret
    }
}
#pragma warning(pop)
