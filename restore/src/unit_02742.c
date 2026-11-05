#include "crt_stream_destructor.h"

void restore_stream_filebuf_destroy(uint32_t self)
{
    volatile int32_t state = -1;
    uint32_t buffer;
    uint32_t release;
    __try {
        restore_stream_write32(self, 0x57053cu);
        state = 0;
        if (restore_stream_read32(self + 48u) != 0u) {
            buffer = restore_stream_read32(self + 16u);
            if (buffer != 0u) {
                release = restore_stream_read32(self + 68u);
                if (release != 0u)
                    restore_stream_release_call(release, buffer);
                else
                    FUN_00561a2b((void *)(uintptr_t)buffer);
            }
        }
        state = -1;
        restore_stream_buffer_destroy(self);
    }
    __finally {
        if (AbnormalTermination() && state >= 0) {
            state = -1;
            restore_stream_buffer_destroy(self);
        }
    }
}
