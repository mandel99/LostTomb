#ifndef PHARAOH_RENDERING_OBELISK_DRAW_H
#define PHARAOH_RENDERING_OBELISK_DRAW_H

#include "pharaoh/rendering/monument_spot_draw.h"
#include "pharaoh/building/type_queries.h"

typedef struct PharaohObeliskDrawContext {
    const PharaohBuildingTypeRecord *buildings;
    const PharaohMonumentSpotDrawContext *spot_draw;
    const PharaohMonumentWorkSpotBinding *small_spots, *large_spots;
    const PharaohMonumentSpotDrawOperations *operations;
    void *user_data;
    uint8_t (*progress)(void *user_data, int32_t building_id);
    int32_t ground_image_first;
} PharaohObeliskDrawContext;

void pharaoh_obelisk_draw_overlay(const PharaohObeliskDrawContext *context,
    int32_t building_id, int32_t x, int32_t y); /* 0x004E5E90 */

#endif
