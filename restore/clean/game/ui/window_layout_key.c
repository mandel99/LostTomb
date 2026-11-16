#include "pharaoh/ui/window_layout_key.h"

#include <stdint.h>

enum {
    PHARAOH_WINDOW_LAYOUT_SPECIAL_KEY = 7,
    PHARAOH_WINDOW_LAYOUT_KEY_OFFSET = 100
};

static int32_t wrap_add(int32_t left, int32_t right)
{
    const uint32_t result_bits = (uint32_t)left + (uint32_t)right;
    if ((result_bits & UINT32_C(0x80000000)) != 0) {
        return (int32_t)((int64_t)result_bits - INT64_C(0x100000000));
    }
    return (int32_t)result_bits;
}

int32_t pharaoh_ui_layout_window_key(int32_t window_key,
    int32_t window_seven_layout_base)
{
    if (window_key == PHARAOH_WINDOW_LAYOUT_SPECIAL_KEY) {
        return wrap_add(window_seven_layout_base,
            PHARAOH_WINDOW_LAYOUT_KEY_OFFSET);
    }
    return window_key;
}
