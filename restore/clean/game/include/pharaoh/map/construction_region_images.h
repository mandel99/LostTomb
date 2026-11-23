#ifndef PHARAOH_MAP_CONSTRUCTION_REGION_IMAGES_H
#define PHARAOH_MAP_CONSTRUCTION_REGION_IMAGES_H

#include "pharaoh/map/surface_tiles.h"

/* Native extent starts at x-1,y-1 and clips to the logical map dimensions. */
void pharaoh_map_refresh_wall_region(PharaohMapSurfaceContext *context,
    int32_t x,int32_t y,int32_t size); /* 0047AFD0 */
void pharaoh_map_refresh_road_square(PharaohMapSurfaceContext *context,
    int32_t x,int32_t y,int32_t size); /* 0047B480 */

#endif
