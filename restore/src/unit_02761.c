#include "crt_stream_constructor.h"

uint32_t restore_stream_buffer_construct(uint32_t self)
{
    restore_stream_write32(self + 12u, 0xffffffffu);
    restore_stream_write32(self, 0x570598u);
    restore_stream_write32(self + 4u, 0u);
    restore_stream_write32(self + 8u, 0u);
    restore_stream_write32(self + 16u, 0u);
    restore_stream_write32(self + 20u, 0u);
    restore_stream_write32(self + 24u, 0u);
    restore_stream_write32(self + 28u, 0u);
    restore_stream_write32(self + 32u, 0u);
    restore_stream_write32(self + 36u, 0u);
    restore_stream_write32(self + 40u, 0u);
    restore_stream_write32(self + 44u, 0u);
    return self;
}
