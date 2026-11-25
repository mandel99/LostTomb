#ifndef PHARAOH_RENDERING_PYRAMID_DRAW_H
#define PHARAOH_RENDERING_PYRAMID_DRAW_H

#include "pharaoh/monuments/pyramid_images.h"
#include "pharaoh/rendering/walker_draw.h"

typedef struct PharaohPyramidDrawContext {
    PharaohBuildingTypeRecord *buildings;
    const int16_t *building_ids,*walker_ids;
    const uint8_t *random;
    const PharaohSpriteGroupRecord *groups;
    PharaohImageDrawContext *draw;
    PharaohWalkerDrawContext *walker;
    const PharaohPyramidImageData *layout;
    const uint32_t *style,*orientation;
    const int32_t *tile_half_width,*tile_half_height;
} PharaohPyramidDrawContext;

void pharaoh_pyramid_draw_part(PharaohPyramidDrawContext *,int32_t image,
    int32_t x,int32_t y,int32_t building_id);

#endif
