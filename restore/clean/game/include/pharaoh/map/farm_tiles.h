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
    const uint8_t *sprites;
    int32_t floodplain_farm_image_first;
    int32_t laborer_work_image_first;
    int32_t laborer_grown_image_first;
    int32_t laborer_harvest_image_first;
    int32_t farm_max_progress;
    int32_t flood_state, game_day, game_tick, locusts_countdown;
} PharaohFarmTileDrawContext;

void pharaoh_map_farm_tiles_meadow(const PharaohFarmTileDrawContext *context,
    int32_t building_id, int32_t x, int32_t y, int32_t image,
    int32_t parts, uint8_t extra); /* 0x00488690 */
void pharaoh_map_farm_tiles_floodplain(
    const PharaohFarmTileDrawContext *context, int32_t building_id,
    int32_t x, int32_t y, int32_t image, int32_t parts,
    uint8_t extra); /* 0x00488840 */

#endif
