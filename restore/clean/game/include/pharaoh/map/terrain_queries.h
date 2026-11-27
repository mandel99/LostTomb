#ifndef PHARAOH_MAP_TERRAIN_QUERIES_H
#define PHARAOH_MAP_TERRAIN_QUERIES_H
#include "pharaoh/map/surface_queries.h"
#include "pharaoh/map/tile_adjacency.h"
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

typedef struct PharaohWaterOrientationMap {
    const uint32_t *terrain;
    const uint8_t *blocking_objects;
    int32_t origin;
    int32_t width;
    int32_t height;
    int32_t view_orientation;
    const int32_t *footprint_offsets;
    const int32_t (*orientation_masks)[9];
    const int32_t (*access_offsets)[2];
    int32_t *raw_orientation;
    int32_t *adjusted_orientation;
} PharaohWaterOrientationMap;
void pharaoh_map_terrain_and_area(PharaohMapTerrainView *map,int32_t x,int32_t y,int32_t size,int32_t radius,uint32_t mask,PharaohMapAreaCursor *cursor);
int32_t pharaoh_map_water_determine_orientation_size3(PharaohWaterOrientationMap *map,int32_t x,int32_t y,uint8_t rotate_with_view);
typedef struct PharaohRoadAccessContext PharaohRoadAccessContext;

int32_t pharaoh_map_has_road_access_granary(
    PharaohRoadAccessContext *context,int32_t x,int32_t y);

int32_t pharaoh_map_general_direction(
    int32_t from_x, int32_t from_y, int32_t to_x, int32_t to_y);

typedef uint32_t (*PharaohMapRingVisitor)(
    uint32_t tile, int32_t x, int32_t y, void *context);

void pharaoh_map_visit_diamond_rings(
    const PharaohMapGridGeometry *geometry, int32_t center_tile,
    int32_t radius, PharaohMapRingVisitor visitor, void *context);

int32_t pharaoh_map_can_place_road_under_canal(
    const uint32_t *terrain, const int32_t *image_ids,
    int32_t canal_group_first_image, int32_t view_orientation,
    int32_t tile);

uint8_t pharaoh_map_tile_is_buildable_for_editor_at(
    const uint32_t *terrain, const int8_t *height_values, int32_t offset);

int32_t pharaoh_map_diagonal_pair_has_mask(
    const uint32_t *terrain, int32_t offset, uint32_t mask);

int32_t pharaoh_map_pattern_has_mask_4000(
    const uint32_t *terrain, int32_t map_origin, int32_t x, int32_t y,
    const int32_t pattern_offsets[][24], int32_t pattern);

typedef struct PharaohConstructionEntryPoints {
    int16_t primary_x[4];
    int16_t primary_y[4];
    int32_t secondary_x[4];
    int32_t secondary_y[4];
} PharaohConstructionEntryPoints;
uint8_t pharaoh_construction_area_has_entry_point(
    const PharaohConstructionEntryPoints *points,
    int32_t x1, int32_t y1, int32_t x2, int32_t y2);
typedef uint32_t (*PharaohMapTerrainPredicate)(
    uint32_t terrain, void *user_data);
int32_t pharaoh_map_region_matches_predicate(
    const PharaohMapTerrainView *map, int32_t x, int32_t y,
    int32_t size, int32_t radius, PharaohMapTerrainPredicate predicate,
    void *user_data, int32_t *result_x, int32_t *result_y);
#endif
