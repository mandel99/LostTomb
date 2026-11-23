#ifndef PHARAOH_MAP_WATER_BUILDING_IMAGES_H
#define PHARAOH_MAP_WATER_BUILDING_IMAGES_H
#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/rendering/water_lift_image.h"

typedef struct PharaohMapSquareCell {
    int32_t grid_offset;
    uint8_t image_offset;
} PharaohMapSquareCell;

typedef struct PharaohBuildingFootprintMap {
    uint32_t *terrain;
    int16_t *building_ids;
    uint8_t *property_flags;
    uint32_t *image_ids;
    uint8_t *tile_states;
    int32_t origin;
    int32_t width;
    int32_t height;
    int32_t view_orientation;
    int32_t *placement_error;
    int32_t *cursor_offset;
    const PharaohMapSquareCell (*square_cells)[6];
} PharaohBuildingFootprintMap;

void pharaoh_map_building_tiles_add_water_size2(
    PharaohBuildingFootprintMap *map, int16_t building_id,
    int32_t x, int32_t y, uint32_t image_id);
void pharaoh_map_building_tiles_add_water_size3(
    PharaohBuildingFootprintMap *map, int16_t building_id,
    int32_t x, int32_t y, uint32_t image_id);
int32_t pharaoh_map_terrain_2x2_is_flooded(
    PharaohFloodTile *const *flood_tiles, int32_t map_origin,
    int32_t x, int32_t y);
int32_t pharaoh_map_building_is_flooded(
    PharaohFloodTile *const *flood_tiles, int32_t map_origin,
    const PharaohBuildingTypeRecord *buildings, int32_t building_id);
void pharaoh_map_update_water_lift_image(PharaohMapSurfaceContext *surface,
    PharaohBuildingFootprintMap *map, PharaohWaterLiftImageState *state,
    int32_t building);
void pharaoh_map_bind_water_footprint(PharaohBuildingFootprintMap *map,
    PharaohMapSurfaceContext *surface);
uint32_t pharaoh_map_water_lift_has_intake(const uint32_t *terrain,
    const PharaohBuildingTypeRecord *buildings,const int32_t offsets[8],
    int32_t building);
#endif
