#include "crt_stream_destructor.h"

void restore_stream_iostream_destroy(uint32_t virtual_base_argument)
{
    volatile int32_t state = -1;
    uint32_t base;
    uint32_t ostream_argument;
    __try {
        base = restore_stream_virtual_base(virtual_base_argument - 20u);
        restore_stream_write32(base, 0x5705d8u);
        state = 0;
        /* Native null-most-derived adjustment: LEA(arg-8), NEG(arg-20),
           SBB/AND, ADD8. Preserve wrapping arithmetic even for arg==20. */
        ostream_argument = ((virtual_base_argument - 20u) != 0u ?
            virtual_base_argument - 8u : 0u) + 8u;
        restore_stream_ostream_destroy(ostream_argument);
        state = -1;
        restore_stream_istream_destroy(virtual_base_argument - 8u);
    }
    __finally {
        if (AbnormalTermination() && state >= 0) {
            state = -1;
            restore_stream_istream_destroy(virtual_base_argument - 8u);
        }
    }
}
