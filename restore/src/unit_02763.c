#include "crt_stream_destructor.h"

void restore_stream_buffer_destroy(uint32_t self)
{
    uint32_t buffer;
    restore_stream_write32(self, 0x570598u);
    (void)restore_stream_buffer_pending(self);
    if (restore_stream_read32(self + 4u) != 0u) {
        buffer = restore_stream_read32(self + 16u);
        if (buffer != 0u)
            FUN_00561a2b((void *)(uintptr_t)buffer);
    }
}
