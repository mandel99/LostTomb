#include "pharaoh/runtime/tax_sentiment_model.h"

#include "pharaoh/runtime/model_text.h"
#include "pharaoh/runtime/settings_ini.h"

enum { NATIVE_SECTION_SEARCH_LIMIT = 299 };

static const char *const tax_sentiment_filenames[
    PHARAOH_MODEL_DIFFICULTY_LEVELS] = {
        "Tax_Sentiment_Model_VeryEasy.txt",
        "Tax_Sentiment_Model_Easy.txt",
        "Tax_Sentiment_Model_Normal.txt",
        "Tax_Sentiment_Model_Hard.txt",
        "Tax_Sentiment_Model_Impossible.txt"
    };

static uint32_t count_model_sections(const char *text, uint32_t length)
{
    const char *cursor = text;
    uint32_t count = 0;
    int32_t remaining = NATIVE_SECTION_SEARCH_LIMIT;
    do {
        const int32_t advance = pharaoh_ini_find_byte(
            cursor, '{', (int32_t)length);
        if (advance == 0) break;
        ++count;
        cursor += advance;
    } while (--remaining > 0);
    return count;
}

int32_t pharaoh_tax_sentiment_model_load(
    PharaohTaxSentimentModelLoadContext *c)
{
    const PharaohTaxSentimentModelOperations *ops = c->operations;
    const char *const filename = tax_sentiment_filenames[c->difficulty];
    if (c->spare_screen == NULL) {
        ops->report_status(ops->user_data, PHARAOH_TAX_MODEL_NO_SPARE_SCREEN);
        return 0;
    }

    ops->clear_bytes(ops->user_data, c->spare_screen,
        PHARAOH_TAX_MODEL_SCRATCH_BYTES, 0);
    if (ops->load_text(ops->user_data, filename, c->spare_screen,
            PHARAOH_TAX_MODEL_SCRATCH_BYTES, 0) == 0) {
        ops->report_status(ops->user_data,
            PHARAOH_TAX_MODEL_FIGURE_FILE_MISSING);
        return 0;
    }

    const char *text = (const char *)c->spare_screen;
    const uint32_t length = ops->text_length(ops->user_data, filename);
    const uint32_t actual = count_model_sections(text, length);
    if (actual != PHARAOH_TAX_MODEL_TAX_PERCENTAGES &&
        ops->continue_after_count_mismatch(ops->user_data, actual + 1,
            PHARAOH_TAX_MODEL_TAX_PERCENTAGES) == 0) {
        ops->report_status(ops->user_data,
            PHARAOH_TAX_MODEL_LINE_COUNT_MISMATCH);
        return 0;
    }

    const char *cursor = text;
    int32_t remaining = NATIVE_SECTION_SEARCH_LIMIT;
    int32_t tax_percentage = 0;
    do {
        if (tax_percentage >= PHARAOH_TAX_MODEL_TAX_PERCENTAGES) break;
        cursor += pharaoh_ini_find_byte(cursor, '{', (int32_t)length);
        for (int32_t model_index = 0;
             model_index < PHARAOH_TAX_MODEL_INDEX_COUNT;
             ++model_index) {
            cursor += pharaoh_model_text_skip_to_number(cursor);
            c->model->by_tax_percentage[tax_percentage][model_index] =
                pharaoh_ini_parse_integer(cursor);
            cursor += pharaoh_ini_find_byte(cursor, ',', (int32_t)length);
        }
        ++tax_percentage;
    } while (--remaining > 0);

    ops->report_status(ops->user_data, PHARAOH_TAX_MODEL_LOADED);
    return 1;
}
