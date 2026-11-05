#include "crt_stream_destructor.h"

void restore_stream_ostream_destroy(uint32_t virtual_base_argument)
{
    uint32_t self = virtual_base_argument - 8u;
    uint32_t base = restore_stream_virtual_base(self);
    restore_stream_write32(base, 0x570588u);
}
