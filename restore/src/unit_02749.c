#include "crt_stream_constructor.h"

uint32_t restore_stream_fstream_construct(uint32_t self,
    uint32_t construct_virtual_base)
{
    volatile uint32_t constructed = 0u;
    volatile int32_t state = -1;
    volatile uint32_t allocation = 0u;
    uint32_t buffer;
    uint32_t base;
    __try {
        if (construct_virtual_base != 0u) {
            restore_stream_write32(self, 0x570578u);
            restore_stream_write32(self + 12u, 0x570570u);
            (void)restore_stream_ios_construct(self + 20u);
            constructed = 1u;
            state = 0;
        }
        allocation = (uint32_t)(uintptr_t)FUN_00561a1d(0x48u);
        state = 1;
        buffer = allocation != 0u
            ? restore_stream_filebuf_construct(allocation) : 0u;
        state = 0;
        (void)restore_stream_iostream_construct(self, buffer, 0u);
        base = restore_stream_virtual_base(self);
        restore_stream_write32(base, 0x57056cu);
        base = restore_stream_virtual_base(self);
        restore_stream_write32(base + 28u, 1u);
        /* The duplicate ownership store and its second vbtable load are
           in the pinned executable. Retain both observable writes. */
        base = restore_stream_virtual_base(self);
        restore_stream_write32(base + 28u, 1u);
    }
    __finally {
        if (AbnormalTermination()) {
            if (state >= 1) {
                state = 0;
                FUN_00561a2b((void *)(uintptr_t)allocation);
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
