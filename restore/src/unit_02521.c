#include "crt_stream_destructor.h"

void restore_stream_fstream_complete_destroy(uint32_t self)
{
    uint32_t virtual_base_argument = self + 20u;
    restore_stream_fstream_destroy(virtual_base_argument);
    restore_stream_ios_destroy(virtual_base_argument);
}
