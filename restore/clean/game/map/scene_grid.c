#include "pharaoh/map/scene_grid.h"


/* Native0x0046F7B0; completed C owner reused with explicit state. */
void pharaoh_map_grid_initialize(PharaohMapGridGeometry *geometry,
    int32_t *row_x_start, int32_t *row_x_end,
    int32_t *row_start_offset)
{
    int32_t y;
    int32_t top_half_left_x;

    geometry->diamond_x_min =
        (PHARAOH_MAP_GRID_WIDTH - geometry->width) / 2;
    geometry->diamond_y_min =
        (PHARAOH_MAP_GRID_WIDTH - geometry->height) / 2;
    geometry->diamond_x_max =
        geometry->diamond_x_min - 1 + geometry->width;
    geometry->diamond_y_max =
        geometry->diamond_y_min - 1 + geometry->height;
    geometry->diamond_x_center =
        (geometry->diamond_x_max + geometry->diamond_x_min) / 2;
    geometry->diamond_y_center =
        (geometry->diamond_y_max + geometry->diamond_y_min) / 2;

    top_half_left_x = geometry->diamond_y_center +
        geometry->diamond_x_min - geometry->diamond_y_min;
    for (y = geometry->diamond_y_min;
         y <= geometry->diamond_y_max; ++y, --top_half_left_x) {
        int32_t left_x = top_half_left_x;
        const int32_t row = y - geometry->diamond_y_min;
        if (y > geometry->diamond_y_center) {
            left_x = geometry->diamond_x_min -
                geometry->diamond_y_center - 1 + y;
        }
        row_x_start[row] = left_x - geometry->diamond_x_min;
        row_x_end[row] = geometry->diamond_x_center * 2 - left_x + 2 -
            geometry->diamond_x_min;
        row_start_offset[row] = left_x + y * PHARAOH_MAP_GRID_WIDTH;
    }
}

/* Native0x004771C0; completed C owner reused with explicit state. */
void pharaoh_map_terrain_shrub_to_tree(uint32_t *terrain,
    int32_t row_count, const int32_t *row_x_start,
    const int32_t *row_x_end, const int32_t *row_start_offset)
{
    int32_t row;
    for (row = 0; row < row_count; ++row) {
        int32_t remaining = row_x_end[row] - row_x_start[row];
        uint32_t *tile;
        if (remaining <= 0) continue;
        tile = terrain + row_start_offset[row];
        do {
            if ((*tile & 0x10u) != 0) {
                *tile = (*tile & ~0x10u) | 1u;
            }
            ++tile;
            --remaining;
        } while (remaining != 0);
    }
}

/* Native0x00476750; completed C owner reused with explicit state. */
void pharaoh_map_terrain_mark_gardens(
    uint32_t *terrain, uint8_t *property_flags,
    uint8_t *overlay_values, const uint32_t *image_ids,
    int32_t row_count, const int32_t *row_x_start,
    const int32_t *row_x_end, const int32_t *row_start_offset,
    int32_t first_garden_image)
{
    const int32_t last_garden_image =
        (int32_t)((uint32_t)first_garden_image + 7u);
    int32_t row;
    for (row = 0; row < row_count; ++row) {
        int32_t tile = row_start_offset[row];
        int32_t remaining = row_x_end[row] - row_x_start[row];
        while (remaining > 0) {
            const int32_t image = (int32_t)image_ids[tile];
            if (image >= first_garden_image && image <= last_garden_image) {
                terrain[tile] |= 0x20u;
                property_flags[tile] &= 0x7fu;
                overlay_values[tile] = 0;
            }
            ++tile;
            --remaining;
        }
    }
}
