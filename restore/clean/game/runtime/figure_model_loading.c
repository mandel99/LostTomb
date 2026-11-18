#include "pharaoh/runtime/figure_model_loading.h"

#include "pharaoh/runtime/model_text.h"
#include "pharaoh/runtime/settings_ini.h"

enum { NATIVE_SECTION_SEARCH_LIMIT = 299 };

static const char *const figure_model_filenames[
    PHARAOH_MODEL_DIFFICULTY_LEVELS] = {
        "Figure_model_VeryEasy.txt",
        "Figure_model_Easy.txt",
        "Figure_model_Normal.txt",
        "Figure_model_Hard.txt",
        "Figure_model_Impossible.txt"
    };

static uint32_t count_model_sections(const char *text, uint32_t length)
{
    const int32_t section_offset = pharaoh_ini_find_key(text,
        "ALL FIGURES", (int32_t)length);
    const char *cursor = text + section_offset;
    uint32_t count = 0;
    int32_t remaining = NATIVE_SECTION_SEARCH_LIMIT;
    do {
        const int32_t advance = pharaoh_ini_find_byte(cursor, '{',
            (int32_t)length);
        if (advance == 0) break;
        ++count;
        cursor += advance;
    } while (--remaining > 0);
    return count;
}

static void read_figure_record(const char **cursor, uint32_t length,
    PharaohFigureModelRecord *record)
{
    int32_t values[PHARAOH_FIGURE_MODEL_FIELD_COUNT];
    for (int32_t field = 0; field < PHARAOH_FIGURE_MODEL_FIELD_COUNT; ++field) {
        *cursor += pharaoh_model_text_skip_to_number(*cursor);
        values[field] = pharaoh_ini_parse_integer(*cursor);
        *cursor += pharaoh_ini_find_byte(*cursor, ',', (int32_t)length);
    }
    *record = (PharaohFigureModelRecord) {
        .category = values[0],
        .max_damage = values[1],
        .attack = values[2],
        .defense = values[3],
        .missile_defense = values[4],
        .missile_attack = values[5],
        .missile_range = values[6],
        .missile_delay = values[7],
        .speed = values[8],
        .invasion_percentage = values[9],
        .fast_run_tile_limit = values[10],
        .active_move_call_limit = values[11],
        .column_m = values[12],
        .column_n = values[13]
    };
}

int32_t pharaoh_figure_model_load(PharaohFigureModelLoadContext *c)
{
    const PharaohFigureModelOperations *ops = c->operations;
    const char *const filename = figure_model_filenames[c->difficulty];
    if (c->spare_screen == NULL) {
        ops->report_status(ops->user_data,
            PHARAOH_FIGURE_MODEL_NO_SPARE_SCREEN);
        return 0;
    }

    ops->clear_bytes(ops->user_data, c->spare_screen,
        PHARAOH_FIGURE_MODEL_SCRATCH_BYTES, 0);
    if (ops->load_text(ops->user_data, filename, c->spare_screen,
            PHARAOH_FIGURE_MODEL_SCRATCH_BYTES, 0) == 0) {
        ops->report_status(ops->user_data,
            PHARAOH_FIGURE_MODEL_FILE_MISSING);
        return 0;
    }

    const char *text = (const char *)c->spare_screen;
    const uint32_t length = ops->text_length(ops->user_data, filename);
    const uint32_t actual_rows = count_model_sections(text, length);
    if (actual_rows != PHARAOH_FIGURE_MODEL_EXPECTED_ROWS &&
        ops->continue_after_count_mismatch(ops->user_data, actual_rows + 1,
            PHARAOH_FIGURE_MODEL_EXPECTED_ROWS) == 0) {
        ops->report_status(ops->user_data,
            PHARAOH_FIGURE_MODEL_ROW_COUNT_MISMATCH);
        return 0;
    }

    const int32_t figures_offset = pharaoh_ini_find_key(text,
        "ALL FIGURES", (int32_t)length);
    const char *cursor = text + figures_offset;
    for (int32_t row = 0; row < PHARAOH_FIGURE_MODEL_RECORD_COUNT; ++row) {
        cursor += pharaoh_ini_find_byte(cursor, '{', (int32_t)length);
        read_figure_record(&cursor, length, &(*c->figure_models)[row]);
    }

    const int32_t enemies_offset = pharaoh_ini_find_key(text,
        "ALL ENEMIES", (int32_t)length);
    cursor = text + enemies_offset;
    for (int32_t nation = 0; nation < PHARAOH_ENEMY_MODEL_NATION_COUNT;
         ++nation) {
        for (int32_t model_type = 0;
             model_type < PHARAOH_ENEMY_MODEL_TYPE_COUNT; ++model_type) {
            cursor += pharaoh_ini_find_byte(cursor, '{', (int32_t)length);
            read_figure_record(&cursor, length,
                &(*c->enemy_models)[nation][model_type]);
        }
    }

    ops->report_status(ops->user_data, PHARAOH_FIGURE_MODEL_LOADED);
    return 1;
}
