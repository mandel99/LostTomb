#include "crt_stream_destructor.h"

uint32_t restore_stream_buffer_pending(uint32_t self)
{
    uint32_t cursor = restore_stream_read32(self + 40u);
    if (cursor < restore_stream_read32(self + 44u))
        return 0xffffffffu;
    cursor = restore_stream_read32(self + 28u);
    if (cursor > restore_stream_read32(self + 24u))
        return 0xffffffffu;
    return 0u;
}
