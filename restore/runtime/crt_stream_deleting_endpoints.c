#include "crt_stream_destructor.h"

uint32_t restore_stream_fstream_scalar_delete(uint32_t virtual_base_argument,
    uint32_t flags)
{
    uint32_t self = virtual_base_argument - 20u;
    restore_stream_fstream_destroy(virtual_base_argument);
    restore_stream_ios_destroy(virtual_base_argument);
    if ((flags & 1u) != 0u)
        FUN_00561a2b((void *)(uintptr_t)self);
    return self;
}

uint32_t restore_stream_ostream_scalar_delete(uint32_t virtual_base_argument,
    uint32_t flags)
{
    uint32_t self = virtual_base_argument - 8u;
    restore_stream_ostream_destroy(virtual_base_argument);
    restore_stream_ios_destroy(virtual_base_argument);
    if ((flags & 1u) != 0u)
        FUN_00561a2b((void *)(uintptr_t)self);
    return self;
}

uint32_t restore_stream_istream_scalar_delete(uint32_t virtual_base_argument,
    uint32_t flags)
{
    uint32_t self = virtual_base_argument - 12u;
    restore_stream_istream_destroy(virtual_base_argument);
    restore_stream_ios_destroy(virtual_base_argument);
    if ((flags & 1u) != 0u)
        FUN_00561a2b((void *)(uintptr_t)self);
    return self;
}

uint32_t restore_stream_iostream_scalar_delete(uint32_t virtual_base_argument,
    uint32_t flags)
{
    uint32_t self = virtual_base_argument - 20u;
    restore_stream_iostream_destroy(virtual_base_argument);
    restore_stream_ios_destroy(virtual_base_argument);
    if ((flags & 1u) != 0u)
        FUN_00561a2b((void *)(uintptr_t)self);
    return self;
}

