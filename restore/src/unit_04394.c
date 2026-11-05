#include "crt_stream_destructor.h"

void restore_stream_cleanup_56e17c(uint32_t native_frame)
{
    restore_stream_buffer_destroy(restore_stream_read32(native_frame - 16u));
}
