#include "crt_stream_constructor.h"

void restore_stream_istream_destroy(uint32_t virtual_base_argument)
{
    uint32_t self = virtual_base_argument - 12u;
    uint32_t base = restore_stream_virtual_base(self);
    restore_stream_write32(base, 0x5705c8u);
}
