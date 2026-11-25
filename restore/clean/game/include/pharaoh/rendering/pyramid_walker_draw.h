#ifndef PHARAOH_RENDERING_PYRAMID_WALKER_DRAW_H
#define PHARAOH_RENDERING_PYRAMID_WALKER_DRAW_H

#include "pharaoh/monuments/stages.h"
#include "pharaoh/runtime/city_view.h"

typedef struct PharaohPyramidWalkerDrawContext {
    const PharaohWalkerQueryRecord *walkers;
    const PharaohBuildingTypeRecord *buildings;
    const PharaohCityViewState *view;
    const PharaohCityProjectionLookup *projection;
    const PharaohCityMapBounds *bounds;
    const PharaohMonumentCoordinateTables *tables;
    const uint32_t *pyramid_style,*anchor_orientation;
} PharaohPyramidWalkerDrawContext;

void pharaoh_pyramid_walker_pixel_offset(PharaohPyramidWalkerDrawContext *,
    int32_t walker_id,int32_t *x,int32_t *y); /*004F0D30*/

#endif
