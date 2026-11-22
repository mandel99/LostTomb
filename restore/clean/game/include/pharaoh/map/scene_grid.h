#ifndef PHARAOH_MAP_SCENE_GRID_H
#define PHARAOH_MAP_SCENE_GRID_H

#include "pharaoh/map/grid_geometry.h"

void pharaoh_map_grid_initialize(PharaohMapGridGeometry *geometry,
    int32_t *row_x_start, int32_t *row_x_end, int32_t *row_start_offset);
void pharaoh_map_terrain_shrub_to_tree(uint32_t *terrain, int32_t row_count,
    const int32_t *row_x_start, const int32_t *row_x_end,
    const int32_t *row_start_offset);
void pharaoh_map_terrain_mark_gardens(uint32_t *terrain,
    uint8_t *property_flags, uint8_t *overlay_values, const uint32_t *image_ids,
    int32_t row_count, const int32_t *row_x_start, const int32_t *row_x_end,
    const int32_t *row_start_offset, int32_t first_garden_image);

#endif
