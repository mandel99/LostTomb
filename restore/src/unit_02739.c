#include "crt_stream_constructor.h"

uint32_t restore_stream_ios_initialize(uint32_t self, uint32_t buffer)
{
    uint32_t old_buffer;
    if (restore_stream_read32(self + 28u) != 0u) {
        old_buffer = restore_stream_read32(self + 4u);
        if (old_buffer != 0u)
            restore_stream_virtual_delete(old_buffer, 1u);
    }
    restore_stream_write32(self + 4u, buffer);
    if (buffer != 0u)
        restore_stream_write32(self + 8u,
            restore_stream_read32(self + 8u) & 0xfffffffbu);
    else
        restore_stream_write32(self + 8u,
            restore_stream_read32(self + 8u) | 4u);
    return buffer;
}
