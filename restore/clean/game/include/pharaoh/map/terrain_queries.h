#ifndef PHARAOH_MAP_TERRAIN_QUERIES_H
#define PHARAOH_MAP_TERRAIN_QUERIES_H
#include "pharaoh/building/type_queries.h"
#include "pharaoh/map/scene_grid.h"
typedef struct PharaohMapRingActorSearch {
    const uint32_t *tile_flags;
    const int16_t *tile_actor_ids;
    const PharaohWalkerQueryRecord *walkers; /* Full388-byte records: next WORD+8, type BYTE+0xA. */
    uint32_t *search_tile;
    int32_t grid_start;
    int32_t width;
    int32_t height;
} PharaohMapRingActorSearch;


int32_t pharaoh_map_ring_actor_search(
    const PharaohMapRingActorSearch *state, int32_t x, int32_t y,
    int32_t kind_count, const uint32_t *kinds, uint32_t flag_mask,
    int32_t *found_x, int32_t *found_y, int32_t remaining_rings,
    int32_t excluded_actor);

int32_t pharaoh_map_pattern_has_water(
    const uint32_t *terrain, int32_t map_origin, int32_t x, int32_t y,
    const int32_t pattern_offsets[][24], int32_t pattern);

#endif
