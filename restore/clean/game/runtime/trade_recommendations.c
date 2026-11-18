#include "pharaoh/runtime/trade_recommendations.h"
#include "pharaoh/runtime/settings_ini.h"

enum { NATIVE_SEARCH_LIMIT = 299 };

static int32_t find_section(const char *text, uint32_t length)
{
    return pharaoh_ini_find_byte(text, '{', (int32_t)length);
}

static uint32_t count_sections(const char *text, uint32_t length)
{
    const char *cursor = text;
    uint32_t count = 0;
    int32_t remaining = NATIVE_SEARCH_LIMIT;
    do {
        const int32_t advance = find_section(cursor, length);
        if (advance == 0) break;
        ++count;
        cursor += advance;
    } while (--remaining > 0);
    return count;
}

int32_t pharaoh_trade_recommendations_load(PharaohTradeModelLoadContext *c)
{
    const PharaohTradeModelOperations *ops = c->operations;
    if (c->spare_screen == NULL) {
        ops->report_status(ops->user_data, PHARAOH_TRADE_MODEL_NO_SPARE_SCREEN);
        return 0;
    }

    ops->clear_bytes(ops->user_data, c->spare_screen,
        PHARAOH_TRADE_MODEL_SCRATCH_BYTES, 0);
    if (ops->load_text(ops->user_data, "trade_recommends.txt",
            c->spare_screen, PHARAOH_TRADE_MODEL_SCRATCH_BYTES, 0) == 0) {
        ops->report_status(ops->user_data, PHARAOH_TRADE_MODEL_FILE_MISSING);
        return 0;
    }

    const char *text = (const char *)c->spare_screen;
    const uint32_t length = ops->text_length(ops->user_data,
        "trade_recommends.txt");
    const uint32_t actual = count_sections(text, length);
    if (actual != PHARAOH_TRADE_RECOMMENDATION_ROWS &&
        ops->continue_after_count_mismatch(ops->user_data, actual + 1,
            PHARAOH_TRADE_RECOMMENDATION_ROWS) == 0) {
        ops->report_status(ops->user_data,
            PHARAOH_TRADE_MODEL_RECOMMENDATION_COUNT_MISMATCH);
        return 0;
    }

    const char *cursor = text;
    int32_t remaining = NATIVE_SEARCH_LIMIT;
    int32_t row = 0;
    do {
        if (row >= PHARAOH_TRADE_RECOMMENDATION_ROWS) break;
        cursor += find_section(cursor, length);
        for (int32_t column = 0; column < PHARAOH_TRADE_RECOMMENDATION_COLUMNS;
             ++column) {
            cursor += pharaoh_model_text_skip_to_number(cursor);
            c->recommendations[row][column] =
                pharaoh_ini_parse_integer(cursor);
            cursor += pharaoh_ini_find_byte(cursor, ',', (int32_t)length);
        }
        ++row;
    } while (--remaining > 0);

    ops->report_status(ops->user_data, PHARAOH_TRADE_MODEL_LOADED);
    return 1;
}
