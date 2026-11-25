#ifndef PHARAOH_RENDERING_ABU_SIMBEL_DRAW_H
#define PHARAOH_RENDERING_ABU_SIMBEL_DRAW_H

#include <stdint.h>
#include "pharaoh/building/type_queries.h"

typedef struct PharaohAbuSimbelDrawContext {
    const PharaohBuildingTypeRecord *buildings;
    int32_t map_orientation;
    int32_t part_a_image_first, part_b_image_first;
    void *user_data;
    const uint8_t *(*built_parts)(void *user_data, int32_t main_building_id);
    void (*draw_image)(void *user_data, int32_t image, int32_t x, int32_t y);
} PharaohAbuSimbelDrawContext;

void pharaoh_abu_simbel_draw_built_parts(
    const PharaohAbuSimbelDrawContext *context,
    int32_t building_id, int32_t x, int32_t y); /* 0x00401F70 */

#endif
