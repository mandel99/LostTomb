#include "pharaoh/core/static_objects.h"

#include <stddef.h>
#include <string.h>

#if UINTPTR_MAX == UINT32_MAX
_Static_assert(offsetof(PharaohScreenFade, reserved_1) == 0x1c &&
    offsetof(PharaohScreenFade, pixels) == 0x24,
    "native fade initialization offsets");
_Static_assert(offsetof(PharaohEmpireRoute, path_length) == 0x134 &&
    offsetof(PharaohEmpireRoute, path_capacity) == 0x138 &&
    offsetof(PharaohEmpireRoute, first_object) == 0x13c &&
    offsetof(PharaohEmpireRoute, point_count) == 0x141 &&
    offsetof(PharaohEmpireRoute, in_use) == 0x142,
    "native route constructor offsets");
#endif

void pharaoh_empire_route_construct(PharaohEmpireRoute *route)
{
    route->path_length = 0;
    route->path = NULL;
    route->point_count = 0;
    route->path_capacity = 0;
    route->in_use = 0;
    route->step = 5;
    route->first_object = -1;
    route->last_object = -1;
    route->kind = 1;
}

void pharaoh_screen_fade_initialize(PharaohScreenFade *fade)
{
    fade->format = -1;
    fade->steps = 0;
    /* Native 00413E91/00413E96 also clear offsets 0x20 and 0x1c.
       Their data types remain unknown; retain the byte storage. */
    memset(fade->reserved_1, 0, sizeof(fade->reserved_1));
    fade->pixels[1] = NULL;
    fade->pixels[0] = NULL;
}

uint32_t pharaoh_native_player_milestone_code_004c5190(
    int16_t first, int16_t second, int16_t third)
{
    uint32_t result = first > 0;
    if (second > 0) result -= 2u;
    if (third > 0) result -= 1u;
    return result;
}

int32_t pharaoh_gods_ra_adjust_value(int32_t value,
    int16_t first, int16_t second, int16_t third)
{
    const uint32_t raw_code =
        pharaoh_native_player_milestone_code_004c5190(first, second, third);
    const int32_t code = (int32_t)raw_code;
    if (value == 0) return 0;
    if (code == 1) {
        if (value == 25) return 40;
        if (value == 15) return 25;
        return value < 15 ? 15 : value;
    }
    if (code > 1) {
        if (value == 25 || value == 15) return 40;
        return value < 15 ? 25 : value;
    }
    if (raw_code == UINT32_MAX) {
        if (value == 40) return 25;
        if (value == 25) return 15;
        if (value == 15) return 0;
        return value;
    }
    if (code >= -1) return value;
    if (value == 40) return 15;
    if (value == 25 || value == 15) return 0;
    return value;
}

void pharaoh_native_flag_from_byte_004e7e20(
    uint32_t *flag, uint8_t enabled)
{
    *flag = enabled != 0;
}

void pharaoh_native_dwords_set_minus_one_00517a20(int32_t values[12996])
{
    int32_t index;
    for (index = 0; index < 12996; ++index) values[index] = -1;
}

void pharaoh_native_state_pair_reset_0041df60(
    uint32_t *first, uint32_t *second)
{
    *first = 0;
    *second = 0;
}

/* Unchanged typed C dependency, native0x004F5020. */
int32_t pharaoh_native_three_nonzero_count_004f5020(
    const int32_t values[3])
{
    int32_t index, count = 0;
    for (index = 0; index < 3; ++index) if (values[index] != 0) ++count;
    return count;
}

/* Complete C translation of Pharaoh.exe 0x00477F60. */
uint8_t pharaoh_native_return_zero_byte_00477f60(void)
{
    return 0;
}

void pharaoh_native_record90_set_word_minus_one_004d1840(
    PharaohNativeRecord90 records[49], int32_t word_offset)
{
    int32_t index;
    for (index = 0; index < 49; ++index)
        *(int16_t *)(records[index].bytes + word_offset) = -1;
}

void pharaoh_native_record30_set_three_words_minus_one_004d1860(
    PharaohNativeRecord30 records[1000], int32_t word_offset)
{
    int32_t index;
    for (index = 0; index < 1000; ++index) {
        int16_t *words = (int16_t *)(records[index].bytes + word_offset);
        words[-1] = -1;
        words[0] = -1;
        words[1] = -1;
    }
}

void pharaoh_native_player_copy_seven_dwords_004d1880(
    uint8_t *players, int32_t count)
{
    int32_t player, index;
    for (player = 0; player < count; ++player) {
        uint8_t *p = players + player * 0x49d8;
        for (index = 0; index < 7; ++index)
            *(uint32_t *)(p + 0x483c + index * 4) =
                *(const uint32_t *)(p + 0x2900 + index * 4);
    }
}

void pharaoh_native_player_expand_word_tables_004d1a70(uint8_t *players)
{
    int32_t player, index;
    for (player = 0; player < 2; ++player) {
        uint8_t *p = players + player * 0x49d8;
        for (index = 0; index < 20; ++index) {
            *(int32_t *)(p + 0x3c64 + index * 4) =
                (int32_t)*(const int16_t *)(p + 0x291c + index * 2);
            *(int32_t *)(p + 0x3d04 + index * 4) =
                (int32_t)*(const int16_t *)(p + 0x2944 + index * 2);
        }
        for (index = 0; index < 20; ++index) {
            *(uint32_t *)(p + 0x3cb4 + index * 4) = 0;
            *(uint32_t *)(p + 0x3d54 + index * 4) = 0;
        }
    }
}

void pharaoh_native_record90_clear_byte_004d3f40(
    PharaohNativeRecord90 records[49], int32_t byte_offset)
{
    int32_t index;
    for (index = 0; index < 49; ++index)
        records[index].bytes[byte_offset] = 0;
}

void pharaoh_native_current_player_block_clear_004d4040(uint8_t *player)
{
    int32_t index;
    *(uint32_t *)(player + 0x298c) = 0;
    *(uint32_t *)(player + 0x2990) = 0;
    player[0x2994] = 0;
    for (index = 0; index < 9; ++index)
        *(uint32_t *)(player + 0x2995 + index * 4) = 0;
}

void pharaoh_native_building_field_clear_004d4080(
    uint8_t *records, int32_t count, int32_t stride, int32_t byte_offset)
{
    int32_t index;
    for (index = 0; index < count; ++index)
        records[index * stride + byte_offset] = 0;
}

void pharaoh_native_two_words_clear_004d40a0(uint8_t *players)
{
    for (int32_t player = 0; player < 2; ++player) {
        uint8_t *const record = players + player * 0x49d8;
        *(uint32_t *)(record + 0x44c8) = 0;
        *(uint32_t *)(record + 0x44cc) = 0;
    }
}

void pharaoh_native_pairs_set_minus_one_004de140(int32_t pairs[4][2])
{
    int32_t index;
    for (index = 0; index < 4; ++index) {
        pairs[index][0] = -1;
        pairs[index][1] = -1;
    }
}

/* Native0053B670, actual child of the city ground pass. */
void pharaoh_native_ambient_scores_clear_0053b670(
    uint32_t scores[12], uint32_t *active)
{
    int32_t index;
    for (index = 0; index < 12; ++index) scores[index] = 0;
    *active = 0;
}

void pharaoh_native_noop_00489500(void)
{
}

int32_t pharaoh_native_undo_contains_004851b0(
    int32_t id, int32_t undo_ready, int32_t mode,
    int32_t count, const int16_t values[525])
{
    int32_t index;
    if (id <= 0 || undo_ready != 1 || mode != 1 || count <= 0) return 0;
    for (index = 0; index < 525; ++index)
        if (values[index] == id) return 1;
    return 0;
}

uint32_t pharaoh_native_return_zero_0047d350(void)
{
    return 0;
}

uint32_t pharaoh_native_type_is_supported_005013a0(uint32_t type)
{
    switch (type) {
    case 1: case 2: case 3: case 4: case 5: case 6:
    case 7: case 8: case 13: case 15: case 17: case 19:
        return 1;
    default:
        return 0;
    }
}

/* Complete C translation of Pharaoh.exe 0x0046A080. */
void pharaoh_native_ten_dwords_clear_0046a080(uint32_t values[10])
{
    int32_t index;
    for (index = 0; index < 10; ++index) values[index] = 0;
}

void pharaoh_native_active_counter_decrement_004ea740(
    int32_t active, int32_t *counter)
{
    if (active != 0) *counter = (int32_t)((uint32_t)*counter - UINT32_C(1));
}

void pharaoh_native_service_records_decay_0041d6f0(int32_t records[69][32])
{
    int32_t record, field;
    for (record = 0; record < 69; ++record) {
        for (field = 2; field <= 6; ++field) records[record][field] = 0;
        records[record][0] /= 2;
    }
}

int32_t pharaoh_native_metric_select_004d5ef0(
    int32_t key, const int32_t values[6])
{
    switch (key) {
    case 1: return values[0]; case 2: return values[1];
    case 3: return values[2]; case 4: return values[3];
    case 7: return values[4]; case 10: return values[5];
    default: return 10;
    }
}

void pharaoh_native_flag_zero_004e7e10(uint32_t *flag)
{
    *flag = 0;
}

int32_t pharaoh_native_record90_has_ready_004b7590(
    const uint8_t *records, int32_t count)
{
    int32_t index;
    for (index = 0; index < count; ++index, records += 0x90)
        if (records[0] == 1 && records[0x42] != 0 &&
            *(const int16_t *)(records + 0x46) > 0) return 1;
    return 0;
}
