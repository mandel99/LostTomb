#ifndef PHARAOH_MAP_SURFACE_QUERIES_H
#define PHARAOH_MAP_SURFACE_QUERIES_H
#include <stdint.h>
#include <stdbool.h>
#include "pharaoh/map/tile_adjacency.h"

typedef struct PharaohMapAreaCursor {
    int32_t x;
    int32_t y;
    int32_t offset;
} PharaohMapAreaCursor;
uint8_t pharaoh_map_sweep_has_passed(int32_t countdown,int32_t sweep_start,int32_t column);
int32_t pharaoh_map_road_canal_crossing_image(uint32_t unused_grid_offset,int32_t image_offset);
bool pharaoh_map_plaza_square_fits(const uint32_t *terrain,
    const uint8_t *property_flags, const uint32_t *image_ids,
    int32_t tile);
uint8_t pharaoh_map_last_tile_is_buildable_for_editor(
    const uint32_t *terrain, const int8_t *height_values, int32_t last_tile);
int32_t pharaoh_map_road_canal_floodplain_image(int32_t image_offset);
int32_t pharaoh_map_terrain_exists_in_area_with(
    const PharaohMapTerrainView *map, int32_t x, int32_t y,
    int32_t size, int32_t radius, uint32_t mask,
    PharaohMapAreaCursor *cursor);
int32_t pharaoh_map_terrain_all_in_area_are(
    const PharaohMapTerrainView *map, int32_t x, int32_t y,
    int32_t size, int32_t radius, uint32_t mask,
    PharaohMapAreaCursor *cursor);

#endif
