#ifndef PHARAOH_RENDERING_GRANARY_DRAW_H
#define PHARAOH_RENDERING_GRANARY_DRAW_H

#include "pharaoh/building/type_queries.h"

typedef struct PharaohGranaryDrawContext {
    const PharaohBuildingTypeRecord *buildings;
    const uint8_t *edge_flags;
    const int32_t *food_count, *food_list;
    int32_t food_pile_image_first;
    void *user_data;
    void (*determine_available_foods)(void *user_data);
    void (*draw_image)(void *user_data, int32_t image, int32_t x, int32_t y);
} PharaohGranaryDrawContext;

void pharaoh_granary_draw_stores(const PharaohGranaryDrawContext *context,
    int32_t building_id, uint32_t tile, int32_t x, int32_t y); /* 0x00442280 */

#endif
