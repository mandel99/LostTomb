#include "crt_stream_io.h"

/* ECX/stack adaptation only; all algorithms are in C. */

__declspec(naked) uint32_t FUN_0056073c(void)
{
    __asm {
        push ecx
        call restore_stream_filebuf_grow
        add esp, 4
        ret
    }
}

__declspec(naked) uint32_t FUN_00560855(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_filebuf_overflow
        add esp, 8
        ret 4
    }
}

__declspec(naked) uint32_t FUN_005608cc(void)
{
    __asm {
        push ecx
        call restore_stream_filebuf_underflow
        add esp, 4
        ret
    }
}

__declspec(naked) uint32_t FUN_0056090c(void)
{
    __asm {
        push dword ptr [esp + 12]
        push dword ptr [esp + 12]
        push dword ptr [esp + 12]
        push ecx
        call restore_stream_filebuf_seek
        add esp, 16
        ret 12
    }
}

__declspec(naked) uint32_t FUN_00560aae(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_ostream_character
        add esp, 8
        ret 4
    }
}

__declspec(naked) uint32_t FUN_00560b2a(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_buffer_put
        add esp, 8
        ret 4
    }
}

__declspec(naked) uint32_t FUN_00560b4c(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_ostream_integer
        add esp, 8
        ret 4
    }
}

__declspec(naked) uint32_t FUN_00560bfd(void)
{
    __asm {
        push ecx
        call restore_stream_ostream_prepare
        add esp, 4
        ret
    }
}

__declspec(naked) void FUN_00560c26(void)
{
    __asm {
        push ecx
        call restore_stream_ostream_suffix
        add esp, 4
        ret
    }
}

__declspec(naked) uint32_t FUN_00560ca6(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_ostream_string
        add esp, 8
        ret 4
    }
}

__declspec(naked) uint32_t FUN_00560ccf(void)
{
    __asm {
        push ecx
        call restore_stream_ostream_flush
        add esp, 4
        ret
    }
}

__declspec(naked) uint32_t FUN_00560d96(void)
{
    __asm {
        push dword ptr [esp + 8]
        push dword ptr [esp + 8]
        push ecx
        call restore_stream_ostream_format
        add esp, 12
        ret 8
    }
}

__declspec(naked) uint32_t FUN_00560fa3(void)
{
    __asm {
        push dword ptr [esp + 8]
        push dword ptr [esp + 8]
        push ecx
        call restore_stream_buffer_write
        add esp, 12
        ret 8
    }
}

__declspec(naked) uint32_t FUN_00560ff9(void)
{
    __asm {
        push dword ptr [esp + 8]
        push dword ptr [esp + 8]
        push ecx
        call restore_stream_buffer_read
        add esp, 12
        ret 8
    }
}

__declspec(naked) uint32_t FUN_005610a7(void)
{
    __asm {
        push ecx
        call restore_stream_buffer_allocate
        add esp, 4
        ret
    }
}

__declspec(naked) uint32_t FUN_005610d4(void)
{
    __asm {
        push dword ptr [esp + 12]
        push dword ptr [esp + 12]
        push dword ptr [esp + 12]
        push ecx
        call restore_stream_buffer_set
        add esp, 16
        ret 12
    }
}

__declspec(naked) uint32_t FUN_0056111c(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_buffer_putback
        add esp, 8
        ret 4
    }
}

__declspec(naked) uint32_t FUN_00561178(void)
{
    __asm {
        push dword ptr [esp + 4]
        push ecx
        call restore_stream_buffer_putback_direct
        add esp, 8
        ret 4
    }
}

__declspec(naked) uint32_t FUN_00560845(void)
{
    __asm {
        push dword ptr [esp + 8]
        push ecx
        call restore_stream_filebuf_set_growth
        add esp, 8
        ret 8
    }
}

__declspec(naked) uint32_t FUN_00560f6f(void)
{
    __asm {
        push dword ptr [esp + 8]
        push dword ptr [esp + 8]
        push ecx
        call restore_stream_buffer_setbuf
        add esp, 12
        ret 8
    }
}

__declspec(naked) uint32_t FUN_00561104(void)
{
    __asm {
        call restore_stream_buffer_default_seek
        ret 12
    }
}

__declspec(naked) uint32_t FUN_0056110a(void)
{
    __asm {
        push dword ptr [esp + 8]
        push dword ptr [esp + 8]
        push ecx
        call restore_stream_buffer_seek_position
        add esp, 12
        ret 8
    }
}

#pragma warning(push)
#pragma warning(disable:4100) /* Naked edges read their documented stack slots. */

__declspec(naked) uint32_t restore_stream_io_virtual0(uint32_t self, uint32_t slot)
{
    __asm {
        mov ecx, dword ptr [esp + 4]
        mov edx, dword ptr [esp + 8]
        mov eax, dword ptr [ecx]
        push eax
        push ecx
        push dword ptr [eax + edx]
        call restore_stream_io_resolve
        add esp, 4
        mov edx, eax
        pop ecx
        pop eax
        call edx
        ret
    }
}

__declspec(naked) uint32_t restore_stream_io_virtual1(uint32_t self, uint32_t slot, uint32_t a)
{
    __asm {
        mov ecx, dword ptr [esp + 4]
        mov edx, dword ptr [esp + 8]
        mov eax, dword ptr [ecx]
        push eax
        push ecx
        push dword ptr [eax + edx]
        call restore_stream_io_resolve
        add esp, 4
        mov edx, eax
        pop ecx
        pop eax
        push dword ptr [esp + 12]
        call edx
        ret
    }
}

__declspec(naked) uint32_t restore_stream_io_virtual2(uint32_t self, uint32_t slot, uint32_t a, uint32_t b)
{
    __asm {
        mov ecx, dword ptr [esp + 4]
        mov edx, dword ptr [esp + 8]
        mov eax, dword ptr [ecx]
        push eax
        push ecx
        push dword ptr [eax + edx]
        call restore_stream_io_resolve
        add esp, 4
        mov edx, eax
        pop ecx
        pop eax
        push dword ptr [esp + 16]
        push dword ptr [esp + 16]
        call edx
        ret
    }
}

__declspec(naked) uint32_t restore_stream_io_virtual3(uint32_t self, uint32_t slot, uint32_t a, uint32_t b, uint32_t c)
{
    __asm {
        mov ecx, dword ptr [esp + 4]
        mov edx, dword ptr [esp + 8]
        mov eax, dword ptr [ecx]
        push eax
        push ecx
        push dword ptr [eax + edx]
        call restore_stream_io_resolve
        add esp, 4
        mov edx, eax
        pop ecx
        pop eax
        push dword ptr [esp + 20]
        push dword ptr [esp + 20]
        push dword ptr [esp + 20]
        call edx
        ret
    }
}

__declspec(naked) uint32_t restore_stream_io_allocate_call(uint32_t target,
    uint32_t bytes, uint32_t live_ecx)
{
    __asm {
        mov eax, dword ptr [esp + 4]
        mov ecx, dword ptr [esp + 12]
        push dword ptr [esp + 8]
        call eax
        add esp, 4
        ret
    }
}

__declspec(naked) void restore_stream_io_release_call(uint32_t target,
    uint32_t buffer)
{
    __asm {
        mov ecx, dword ptr [esp + 4]
        mov eax, dword ptr [esp + 8]
        push eax
        call ecx
        add esp, 4
        ret
    }
}
#pragma warning(pop)
