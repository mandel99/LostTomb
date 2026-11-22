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

#endif
