#include "crt_stream_constructor.h"

uint32_t restore_stream_filebuf_construct(uint32_t self)
{
    (void)restore_stream_buffer_construct(self);
    restore_stream_write32(self, 0x57053cu);
    restore_stream_write32(self + 48u, 1u);
    restore_stream_write32(self + 52u, 1u);
    restore_stream_write32(self + 60u, 0u);
    restore_stream_write32(self + 64u, 0u);
    restore_stream_write32(self + 68u, 0u);
    return self;
}
