#include "crt_stream_destructor.h"

void restore_stream_fstream_destroy(uint32_t virtual_base_argument)
{
    uint32_t base = restore_stream_virtual_base(virtual_base_argument - 20u);
    restore_stream_write32(base, 0x57056cu);
    restore_stream_iostream_destroy(virtual_base_argument);
}
