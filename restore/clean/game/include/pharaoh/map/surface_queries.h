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

int32_t pharaoh_map_terrain_any_tile_in_square_with(
    const PharaohMapTerrainView *map, int32_t x, int32_t y,
    int32_t size, uint32_t mask);
int32_t pharaoh_map_terrain_any_tile_in_square_without(
    const PharaohMapTerrainView *map, int32_t x, int32_t y,
    int32_t size, uint32_t mask);
int32_t pharaoh_map_terrain_row_has_two(
    const PharaohMapTerrainView *map, int32_t x, int32_t y, uint32_t mask);
int32_t pharaoh_map_terrain_column_has_two(
    const PharaohMapTerrainView *map, int32_t x, int32_t y, uint32_t mask);
uint8_t pharaoh_map_sweep_matches_tile(
    int32_t countdown, int32_t sweep_start, int32_t tile);
int32_t pharaoh_map_find_free_terrain(const PharaohMapTerrainView *map,int32_t x,int32_t y,
    int32_t size,int32_t radius,int32_t excluded,PharaohMapAreaCursor *cursor);

#endif
