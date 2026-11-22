#include "pharaoh/core/static_objects.h"

#include <stddef.h>

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
