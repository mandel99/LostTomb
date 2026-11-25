#ifndef PHARAOH_RENDERING_BUILDING_STOCK_DRAW_H
#define PHARAOH_RENDERING_BUILDING_STOCK_DRAW_H

#include "pharaoh/building/type_queries.h"
#include "pharaoh/rendering/image_draw.h"

typedef struct PharaohBuildingStockDrawContext {
    const PharaohBuildingTypeRecord *buildings;
    const PharaohSpriteGroupRecord *groups;
    PharaohImageDrawContext *draw;
    const uint32_t *orientation;
} PharaohBuildingStockDrawContext;

void pharaoh_building_draw_stock(PharaohBuildingStockDrawContext *,
    int32_t building_id,int32_t x,int32_t y);

#endif
