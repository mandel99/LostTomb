#include "crt_stream_constructor.h"

void restore_stream_ios_destroy(uint32_t self)
{
    uint32_t buffer;
    uint32_t owns_buffer = restore_stream_read32(self + 28u);
    restore_stream_write32(self, 0x570534u);
    if (owns_buffer != 0u) {
        buffer = restore_stream_read32(self + 4u);
        if (buffer != 0u)
            restore_stream_virtual_delete(buffer, 1u);
    }
    restore_stream_write32(self + 4u, 0u);
    restore_stream_write32(self + 8u, 4u);
}
