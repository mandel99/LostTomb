#include "pharaoh/runtime/model_text.h"

enum { NATIVE_NUMBER_SCAN_LIMIT = 999 };

uint32_t pharaoh_model_text_skip_to_number(const char *text)
{
    uint32_t offset;
    for (offset = 0; offset < NATIVE_NUMBER_SCAN_LIMIT; ++offset) {
        const uint8_t value = (uint8_t)text[offset];
        if ((value >= (uint8_t)'0' && value <= (uint8_t)'9') ||
            value == (uint8_t)'-') {
            return offset;
        }
    }
    return NATIVE_NUMBER_SCAN_LIMIT;
}
