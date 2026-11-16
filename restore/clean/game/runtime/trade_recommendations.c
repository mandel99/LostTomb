#include "pharaoh/runtime/trade_recommendations.h"

enum { NATIVE_SEARCH_LIMIT = 299, NATIVE_NUMBER_SKIP_LIMIT = 999 };

static uint32_t find_section(const PharaohTradeModelOperations *ops,
    const char *text, uint32_t length)
{
    const int32_t offset = ops->find_byte(ops->user_data, text, (uint8_t)'{', length);
    return (uint32_t)offset;
}

static uint32_t count_sections(const PharaohTradeModelOperations *ops,
    const char *text, uint32_t length)
{
    const char *cursor = text;
    uint32_t count = 0;
    int32_t remaining = NATIVE_SEARCH_LIMIT;
    do {
        const uint32_t advance = find_section(ops, cursor, length);
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
    const uint32_t length = ops->text_length(ops->user_data, text);
    uint32_t actual = count_sections(ops, text, length);
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
        cursor += find_section(ops, cursor, length);
        for (int32_t column = 0; column < PHARAOH_TRADE_RECOMMENDATION_COLUMNS;
             ++column) {
            cursor += ops->skip_to_number(ops->user_data, cursor);
            c->recommendations[row][column] =
                ops->parse_number(ops->user_data, cursor);
            cursor += (uint32_t)ops->find_byte(ops->user_data, cursor,
                (uint8_t)',', length);
        }
        ++row;
    } while (--remaining > 0);

    ops->report_status(ops->user_data, PHARAOH_TRADE_MODEL_LOADED);
    return 1;
}
