#include "crt_stream_constructor.h"

uint32_t restore_stream_iostream_construct(uint32_t self, uint32_t buffer,
    uint32_t construct_virtual_base)
{
    volatile uint32_t constructed = 0u;
    volatile int32_t state = -1;
    uint32_t base;
    __try {
        if (construct_virtual_base != 0u) {
            restore_stream_write32(self, 0x5705e4u);
            restore_stream_write32(self + 12u, 0x5705dcu);
            (void)restore_stream_ios_construct(self + 20u);
            constructed = 1u;
            state = 0;
        }
        (void)restore_stream_istream_construct(self, buffer, 0u);
        state = 1;
        (void)restore_stream_ostream_construct(self + 12u, buffer, 0u);
        base = restore_stream_virtual_base(self);
        restore_stream_write32(base, 0x5705d8u);
    }
    __finally {
        if (AbnormalTermination()) {
            if (state >= 1) {
                state = 0;
                /* 56E222 calls 56123E with ECX=self+0x0C, even though
                   the most-derived virtual base is at self+0x14. */
                restore_stream_istream_destroy(self + 12u);
            }
            if (state >= 0) {
                state = -1;
                if ((constructed & 1u) != 0u)
                    restore_stream_ios_destroy(self + 20u);
            }
        }
    }
    return self;
}
