#include "crt_stream_constructor.h"

uint32_t restore_stream_istream_construct(uint32_t self, uint32_t buffer,
    uint32_t construct_virtual_base)
{
    volatile uint32_t constructed = 0u;
    volatile int32_t state = -1;
    uint32_t base;
    __try {
        if (construct_virtual_base != 0u) {
            restore_stream_write32(self, 0x5705ccu);
            (void)restore_stream_ios_construct(self + 12u);
            constructed = 1u;
            state = 0;
        }
        base = restore_stream_virtual_base(self);
        restore_stream_write32(base, 0x5705c8u);
        base = restore_stream_virtual_base(self);
        (void)restore_stream_ios_initialize(base, buffer);
        base = restore_stream_virtual_base(self);
        restore_stream_write32(base + 36u,
            restore_stream_read32(base + 36u) | 1u);
        restore_stream_write32(self + 8u, 0u);
        restore_stream_write32(self + 4u, 0u);
    }
    __finally {
        if (AbnormalTermination() && state >= 0 && (constructed & 1u) != 0u)
            restore_stream_ios_destroy(self + 12u);
    }
    return self;
}
