#include "pharaoh/runtime/building_model_loading.h"

#include "pharaoh/runtime/model_text.h"
#include "pharaoh/runtime/settings_ini.h"

enum { NATIVE_SECTION_SEARCH_LIMIT = 299 };

static const char *const pharaoh_model_filenames[
    PHARAOH_MODEL_DIFFICULTY_LEVELS] = {
        "Pharaoh_Model_VeryEasy.txt",
        "Pharaoh_Model_Easy.txt",
        "Pharaoh_Model_Normal.txt",
        "Pharaoh_Model_Hard.txt",
        "Pharaoh_Model_Impossible.txt"
    };

static uint32_t count_model_sections(const char *text, uint32_t length,
                                    const char *section_name)
{
    const int32_t section_offset = pharaoh_ini_find_key(text, section_name,
        (int32_t)length);
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

static void read_building_record(const char **cursor, uint32_t length,
    PharaohBuildingModelRecord *record)
{
    int32_t values[PHARAOH_BUILDING_MODEL_FIELD_COUNT];
    for (int32_t field = 0; field < PHARAOH_BUILDING_MODEL_FIELD_COUNT; ++field) {
        *cursor += pharaoh_model_text_skip_to_number(*cursor);
        values[field] = pharaoh_ini_parse_integer(*cursor);
        *cursor += pharaoh_ini_find_byte(*cursor, ',', (int32_t)length);
    }
    *record = (PharaohBuildingModelRecord) {
        .cost = values[0],
        .desirability_value = values[1],
        .desirability_step = values[2],
        .desirability_step_size = values[3],
        .desirability_range = values[4],
        .laborers = values[5],
        .fire_risk_increment = values[6],
        .damage_risk_increment = values[7],
        .walker_cost = values[8],
        .service_reduction = values[9]
    };
}

static void read_house_record(const char **cursor, uint32_t length,
    PharaohHouseModel *record)
{
    int32_t values[PHARAOH_HOUSE_MODEL_FIELD_COUNT];
    for (int32_t field = 0; field < PHARAOH_HOUSE_MODEL_FIELD_COUNT; ++field) {
        *cursor += pharaoh_model_text_skip_to_number(*cursor);
        values[field] = pharaoh_ini_parse_integer(*cursor);
        *cursor += pharaoh_ini_find_byte(*cursor, ',', (int32_t)length);
    }
    *record = (PharaohHouseModel) {
        .devolve_desirability = values[0],
        .evolve_desirability = values[1],
        .entertainment = values[2],
        .water = values[3],
        .religion = values[4],
        .education = values[5],
        .market = values[6],
        .dentist = values[7],
        .courthouse = values[8],
        .health = values[9],
        .food_types = values[10],
        .pottery = values[11],
        .linen = values[12],
        .jewelry = values[13],
        .beer = values[14],
        .crime_growth = values[15],
        .crime_base = values[16],
        .prosperity = values[17],
        .max_people = values[18],
        .tax_multiplier = values[19],
        .malaria_risk = values[20],
        .disease_risk = values[21]
    };
}

int32_t pharaoh_building_model_load(PharaohBuildingModelLoadContext *c)
{
    const PharaohBuildingModelOperations *ops = c->operations;
    const char *const filename = pharaoh_model_filenames[c->difficulty];
    if (c->spare_screen == NULL) {
        ops->report_status(ops->user_data,
            PHARAOH_BUILDING_MODEL_NO_SPARE_SCREEN);
        return 0;
    }

    ops->clear_bytes(ops->user_data, c->spare_screen,
        PHARAOH_BUILDING_MODEL_SCRATCH_BYTES, 0);
    if (ops->load_text(ops->user_data, filename, c->spare_screen,
            PHARAOH_BUILDING_MODEL_SCRATCH_BYTES, 0) == 0) {
        ops->report_status(ops->user_data,
            PHARAOH_BUILDING_MODEL_FILE_MISSING);
        return 0;
    }

    const char *text = (const char *)c->spare_screen;
    const uint32_t length = ops->text_length(ops->user_data, filename);
    const uint32_t actual_rows = count_model_sections(text, length,
        "ALL BUILDINGS");
    if (actual_rows != PHARAOH_BUILDING_MODEL_EXPECTED_ROWS &&
        ops->continue_after_count_mismatch(ops->user_data, actual_rows + 1,
            PHARAOH_BUILDING_MODEL_EXPECTED_ROWS) == 0) {
        ops->report_status(ops->user_data,
            PHARAOH_BUILDING_MODEL_ROW_COUNT_MISMATCH);
        return 0;
    }

    const int32_t buildings_offset = pharaoh_ini_find_key(text,
        "ALL BUILDINGS", (int32_t)length);
    const char *cursor = text + buildings_offset;
    for (int32_t row = 0; row < PHARAOH_BUILDING_MODEL_RECORD_COUNT; ++row) {
        cursor += pharaoh_ini_find_byte(cursor, '{', (int32_t)length);
        read_building_record(&cursor, length, &c->building_models[row]);
    }

    const int32_t houses_offset = pharaoh_ini_find_key(text, "ALL HOUSES",
        (int32_t)length);
    cursor = text + houses_offset;
    for (int32_t row = 0; row < PHARAOH_HOUSE_MODEL_RECORD_COUNT; ++row) {
        cursor += pharaoh_ini_find_byte(cursor, '{', (int32_t)length);
        read_house_record(&cursor, length, &c->house_models[row]);
    }

    ops->report_status(ops->user_data, PHARAOH_BUILDING_MODEL_LOADED);
    return 1;
}
