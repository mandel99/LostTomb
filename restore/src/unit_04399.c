#include "crt_stream_destructor.h"

void restore_stream_cleanup_56e208(uint32_t native_frame)
{
    if ((restore_stream_read32(native_frame - 16u) & 1u) != 0u)
        restore_stream_ios_destroy(restore_stream_read32(native_frame - 20u) + 20u);
}
