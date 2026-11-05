#include "crt_stream_destructor.h"

void restore_stream_cleanup_56e222(uint32_t native_frame)
{
    restore_stream_istream_destroy(restore_stream_read32(native_frame - 20u) + 12u);
}
