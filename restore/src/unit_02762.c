#include "crt_stream_destructor.h"

uint32_t restore_stream_buffer_scalar_delete(uint32_t self, uint32_t flags)
{
    restore_stream_buffer_destroy(self);
    if ((flags & 1u) != 0u)
        FUN_00561a2b((void *)(uintptr_t)self);
    return self;
}
