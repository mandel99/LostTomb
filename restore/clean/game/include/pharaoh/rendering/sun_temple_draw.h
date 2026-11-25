#ifndef PHARAOH_RENDERING_SUN_TEMPLE_DRAW_H
#define PHARAOH_RENDERING_SUN_TEMPLE_DRAW_H

#include <stdint.h>

#include "pharaoh/building/type_queries.h"

typedef struct PharaohSunTempleDrawContext {
    const PharaohBuildingTypeRecord *buildings;
    int32_t block_image_first;
    void *user_data;
    void (*draw_image)(void *user_data, int32_t image, int32_t x, int32_t y);
} PharaohSunTempleDrawContext;

void pharaoh_sun_temple_draw_stage_blocks(
    const PharaohSunTempleDrawContext *context,
    int32_t building_id, int32_t x, int32_t y); /* 0x00542E10 */

#endif
