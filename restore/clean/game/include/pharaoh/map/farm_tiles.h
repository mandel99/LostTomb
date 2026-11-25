#ifndef PHARAOH_MAP_FARM_TILES_H
#define PHARAOH_MAP_FARM_TILES_H

#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/rendering/image_draw.h"

typedef struct PharaohFarmTileDrawContext {
    PharaohMapSurfaceContext *surface;
    PharaohImageDrawContext *draw;
    int32_t meadow_farm_image_first;
    int32_t tile_half_width, tile_half_height, tile_height;
    void *user_data;
    int32_t (*has_water_access)(void *user_data, int32_t building_id);
} PharaohFarmTileDrawContext;

void pharaoh_map_farm_tiles_meadow(const PharaohFarmTileDrawContext *context,
    int32_t building_id, int32_t x, int32_t y, int32_t image,
    int32_t parts, uint8_t extra); /* 0x00488690 */

#endif
