#include "pharaoh/core/text_length.h"

enum {
    PHARAOH_TEXT_LENGTH_SCAN_LIMIT = 10000
};

/* 004cc000 counts through the first NUL or 10,000 non-NUL bytes. */
int32_t pharaoh_text_length(const char *text)
{
    int32_t length = 0;
    int32_t remaining = PHARAOH_TEXT_LENGTH_SCAN_LIMIT;

    do {
        if (*text == '\0') {
            return length;
        }

        ++text;
        ++length;
        --remaining;
    } while (remaining > 0);

    return length;
}
