#ifndef PHARAOH_MAP_TILE_ADJACENCY_H
#define PHARAOH_MAP_TILE_ADJACENCY_H

#include <stdint.h>
#include "pharaoh/building/type_queries.h"

typedef struct PharaohMapTerrainView {
    uint32_t *terrain;
    int32_t origin;
    int32_t width;
    int32_t height;
} PharaohMapTerrainView;

uint8_t pharaoh_map_terrain_count_neighbours_with(
    const uint32_t *terrain, int32_t offset, uint32_t mask);
uint8_t pharaoh_map_terrain_mask_has_open_corner(
    const uint32_t *terrain, int32_t tile, uint32_t mask,
    uint8_t check_opposite_diagonals);

typedef struct PharaohMapTileAreaOffset {
    uint32_t grid_offset, reserved;
} PharaohMapTileAreaOffset;

typedef struct PharaohMapTileArea {
    const PharaohMapTerrainView *map;
    const uint32_t *image_ids;
    const PharaohMapTileAreaOffset (*offsets)[6];
    uint32_t *current_tile;
} PharaohMapTileArea;

void pharaoh_map_adjacent_terrain_any(uint32_t out[8],
    const uint32_t *terrain, int32_t tile, uint32_t mask);
void pharaoh_map_adjacent_terrain_all(uint32_t out[8],
    const uint32_t *terrain, int32_t tile, uint32_t mask);
void pharaoh_map_adjacent_terrain_include_exclude(uint32_t out[8],
    const uint32_t *terrain, int32_t tile, uint32_t include, uint32_t exclude);
void pharaoh_map_adjacent_terrain_add_bits(uint32_t out[8],
    const uint32_t *terrain, int32_t tile, uint32_t mask, uint32_t shift);
void pharaoh_map_adjacent_road_flags(uint32_t out[8],
    const uint32_t *terrain, const uint8_t *property_flags, int32_t tile);
uint32_t pharaoh_map_adjacent_floodplain_cardinals(uint32_t out[8],
    const uint32_t *terrain, int32_t tile);
void pharaoh_map_adjacent_add_canal_flags(uint32_t out[8],
    const uint32_t *terrain, int32_t tile);
int32_t pharaoh_map_terrain_rectangle_fits(const PharaohMapTileArea *c,
    int32_t x, int32_t y, int32_t width, int32_t height,
    uint32_t flags, uint32_t image_plus_one);
int32_t pharaoh_map_terrain_least_connected_neighbor(
    const uint32_t *terrain, int32_t tile, uint32_t mask);

typedef struct PharaohMapRockVariantContext {
    uint32_t *adjacent; /*16 DWORDs; native clears the eight following slots too */
    uint32_t *current_tile;
    const uint32_t *terrain;
    const int16_t *building_ids;
    const PharaohBuildingTypeRecord *buildings;
    const uint8_t *random;
    int32_t orientation;
} PharaohMapRockVariantContext;

extern const int32_t pharaoh_royal_tomb_rock_offsets[14][4];

int32_t pharaoh_map_royal_tomb_rock_variant(PharaohMapRockVariantContext *c,
    uint32_t tile);

void pharaoh_map_adjacent_flags_invert(uint32_t flags[8]);
void pharaoh_map_adjacent_flags_normalize(uint32_t flags[8]);
int32_t pharaoh_map_adjacent_flags_count(const uint32_t flags[8]);

#endif
