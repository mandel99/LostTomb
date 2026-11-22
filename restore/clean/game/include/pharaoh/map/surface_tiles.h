#ifndef PHARAOH_MAP_SURFACE_TILES_H
#define PHARAOH_MAP_SURFACE_TILES_H

#include "pharaoh/assets/sprite_libraries.h"
#include "pharaoh/map/image_context.h"
#include "pharaoh/map/tile_adjacency.h"
#include "pharaoh/map/grid_geometry.h"
#include "pharaoh/map/ring_types.h"
#include "pharaoh/monuments/pyramid_tile_query.h"
#include "pharaoh/simulation/flood_simulation.h"

typedef struct PharaohPyramidTilePlacement {
    uint8_t found;
    uint8_t reserved_01[3];
    uint32_t target_building, missing_type, missing_tile, size;
    uint32_t span, terminal_tile, step, anchor_tile;
} PharaohPyramidTilePlacement;

_Static_assert(sizeof(PharaohPyramidTilePlacement)==36 &&
    offsetof(PharaohPyramidTilePlacement,anchor_tile)==32,
    "native pyramid placement controls EAAE84..EAAEA4");

typedef struct PharaohMapBuildingUndo {
    const PharaohBuildingTypeRecord *live;
    PharaohBuildingTypeRecord *backup;
    int16_t *ids; /*525 WORDs */
    uint32_t *count, *available;
} PharaohMapBuildingUndo;

typedef struct PharaohMapSurfaceRenderOperations {
    void *user_data;
    void (*project_tile)(void *user_data,int32_t x,int32_t y,
        int32_t *screen_x,int32_t *screen_y);
    int32_t (*draw_masked_sprite)(void *user_data,int32_t image,
        int32_t x,int32_t y,uint32_t mask);
} PharaohMapSurfaceRenderOperations;

typedef struct PharaohMapSurfaceContext {
    PharaohMapTerrainView *map;
    const PharaohMapGridGeometry *geometry;
    const int32_t *border_size, *orientation;
    const uint32_t *building_orientation;
    const uint32_t *editor_active, *force_paved_roads;
    const int16_t *scenario_entry_x, *scenario_entry_y, *scenario_exit_x, *scenario_exit_y;
    int32_t *entry_x, *entry_y, *exit_x, *exit_y, *entry_tile, *exit_tile;
    PharaohMapImageContextRecord *water_context; /*48 rows */
    PharaohMapImageContextRecord *bank_context; /*8 groups of24 rows */
    PharaohMapImageContextRecord *paved_road_context; /*48 rows */
    PharaohMapImageContextRecord *dirt_road_context; /*17 rows */
    const int32_t *row_x_start, *row_x_end, *row_start_offset;
    const int8_t *height_values;
    int32_t *area_x, *area_y;
    uint32_t *channels_refresh_pending;
    PharaohMapImageContextRecord *fault_context; /*17 rows */
    PharaohMapImageContextRecord *canal_context; /*48 rows */
    PharaohMapImageContextRecord *flood_canal_context; /*15 rows */
    PharaohMapImageContextRecord *channel_context; /*16 rows */
    uint32_t *current_tile, *adjacent;
    int32_t *scan_x, *scan_y;
    uint32_t *image_ids;
    uint8_t *property_flags, *edge_flags, *canal_water;
    const uint8_t *fertility, *variation_tag, *meadow_value, *random;
    int16_t *building_ids;
    const int16_t *blocked_grid; /*native WORD grid B350DC, zero test only */
    PharaohBuildingTypeRecord *buildings;
    PharaohFloodTile *const *flood_tiles;
    const PharaohSpriteGroupRecord *groups;
    const PharaohMapTileAreaOffset (*footprints)[6];
    uint32_t *placement_failed;
    PharaohMapImageContextRecord *floodplain_edges; /*26 mutable16-byte records */
    PharaohMapImageSelection *selection;
    const PharaohRingOffsetTables *rings;
    const int16_t *distance_offsets; /*624 native WORD offsets */
    const int32_t *sweep_countdown, *sweep_start;
    const uint32_t *pyramid_style;
    const PharaohPyramidSpecialStageTable *pyramid_regular, *pyramid_style_five;
    uint32_t *pyramid_queue_count, *pyramid_queue; /*four roots */
    const uint32_t *pyramid_stage3_offset;
    const int32_t *pyramid_complex_offsets;
    PharaohPyramidTilePlacement *pyramid_placement;
    PharaohMapBuildingUndo *undo;
    uint32_t *snapshot_calls;
    const int32_t *preview_count;
    const int32_t (*preview_offsets)[2];
    const PharaohMapSurfaceRenderOperations *render; /*required for mode1 */
} PharaohMapSurfaceContext;

void pharaoh_map_place_region_images(PharaohMapSurfaceContext *c,
    int32_t building,int32_t x,int32_t y,int32_t width,int32_t height,
    uint32_t image,uint32_t flags);
int32_t pharaoh_map_empty_rectangle_fits(PharaohMapSurfaceContext *c,
    int32_t x,int32_t y,int32_t width,int32_t height,int32_t mask,
    int32_t required_image,uint8_t allow_floodplain);
void pharaoh_map_refresh_empty_region(PharaohMapSurfaceContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1,
    uint8_t preserve_clear,uint8_t exact_rectangle);
void pharaoh_map_refresh_trees(PharaohMapSurfaceContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_map_refresh_meadow(PharaohMapSurfaceContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_map_set_tree_image(PharaohMapSurfaceContext *c,
    int32_t x,int32_t y,int32_t set);
void pharaoh_map_set_meadow_image(PharaohMapSurfaceContext *c,
    int32_t x,int32_t y,int32_t set);
void pharaoh_map_set_vegetation_image(PharaohMapSurfaceContext *c,uint32_t tile);
uint8_t pharaoh_map_ring_is_wooded(PharaohMapSurfaceContext *c,
    int32_t x,int32_t y,int32_t ring);
void pharaoh_pyramid_queue_surface_refresh(PharaohMapSurfaceContext *c,int32_t building);
void pharaoh_pyramid_publish_surface_queue(PharaohMapSurfaceContext *c);
uint8_t pharaoh_pyramid_set_construction_tile_image(PharaohMapSurfaceContext *c,
    int32_t building,uint32_t tile,int32_t stage,uint8_t action);
void pharaoh_pyramid_map_segment_update(PharaohMapSurfaceContext *c,
    int32_t x,int32_t y,int32_t mode,uint32_t mask);
uint32_t pharaoh_map_building_undo_capture(PharaohMapBuildingUndo *c,int32_t building);


void pharaoh_map_refresh_surrounding_empty(PharaohMapSurfaceContext *c,int32_t x,int32_t y,int32_t size);
void pharaoh_map_place_surface_square(PharaohMapSurfaceContext *c,uint16_t building,
    int32_t x,int32_t y,int32_t width,int32_t height,uint32_t image,uint32_t flags);
void pharaoh_map_refresh_gardens(PharaohMapSurfaceContext *c);
void pharaoh_map_refresh_rocks(PharaohMapSurfaceContext *c);
void pharaoh_map_refresh_dunes(PharaohMapSurfaceContext *c);
void pharaoh_map_refresh_canals(PharaohMapSurfaceContext *c);
void pharaoh_map_refresh_paved_region(PharaohMapSurfaceContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1,uint32_t clear);
void pharaoh_map_refresh_channel_region(PharaohMapSurfaceContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1,uint32_t refresh);
void pharaoh_map_adjacent_channel_flags(PharaohMapSurfaceContext *c);
void pharaoh_map_adjacent_add_channel_side(PharaohMapSurfaceContext *c,
    int32_t direction,uint32_t *count);
void pharaoh_map_set_fault_image(PharaohMapSurfaceContext *c,int32_t x,int32_t y,int32_t set);
void pharaoh_map_refresh_faults(PharaohMapSurfaceContext *c,int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_map_set_rubble_image(PharaohMapSurfaceContext *c,int32_t x,int32_t y);
void pharaoh_map_refresh_rubble(PharaohMapSurfaceContext *c,int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_map_set_channel_image(PharaohMapSurfaceContext *c,int32_t full);
void pharaoh_map_update_channel_images(PharaohMapSurfaceContext *c,int32_t x0,int32_t y0,int32_t x1,int32_t y1);


void pharaoh_map_update_entry_exit_flags(PharaohMapSurfaceContext *c,int32_t remove);
void pharaoh_map_refresh_floodplain(PharaohMapSurfaceContext *c,int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_map_set_floodplain_image(PharaohMapSurfaceContext *c,int32_t x,int32_t y,int32_t set);
void pharaoh_map_refresh_marshland(PharaohMapSurfaceContext *c,int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_map_set_marshland_image(PharaohMapSurfaceContext *c,int32_t x,int32_t y,int32_t set);
void pharaoh_map_refresh_water(PharaohMapSurfaceContext *c,int32_t x0,int32_t y0,int32_t x1,int32_t y1,uint32_t expand);
void pharaoh_map_set_water_image(PharaohMapSurfaceContext *c,int32_t x,int32_t y,int32_t set);
void pharaoh_map_update_deepwater(PharaohMapSurfaceContext *c,int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_map_shrink_deepwater_around(PharaohMapSurfaceContext *c,int32_t x,int32_t y);
void pharaoh_map_refresh_roads(PharaohMapSurfaceContext *c);
void pharaoh_map_refresh_road_region(PharaohMapSurfaceContext *c,int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_map_set_road_image(PharaohMapSurfaceContext *c);
void pharaoh_map_adjacent_add_gatehouse_flags(PharaohMapSurfaceContext *c);
void pharaoh_map_refresh_all_tiles(PharaohMapSurfaceContext *c);

#endif
