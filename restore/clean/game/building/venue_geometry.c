#include "pharaoh/building/venue_geometry.h"

enum { MAP_WIDTH = 228, TERRAIN_WATER = 0x40 };

/* Complete logical-byte C translation of Pharaoh.exe 0x0043F250. */
bool pharaoh_construction_venue_is_crossing(const uint32_t *terrain,
    int32_t map_grid_start, int32_t x, int32_t y,
    int32_t origin_x, int32_t origin_y, int32_t size)
{
    static const int32_t direction_offsets[4] = {
        -MAP_WIDTH, 1, MAP_WIDTH, -1
    };
    int32_t side_lengths[4];
    int32_t side;
    int32_t water_sides = 0;
    int32_t nonzero_water_sides = 0;
    int32_t last_non_water_side = 0;
    const int32_t grid_offset = map_grid_start + y * MAP_WIDTH + x;
    if ((terrain[grid_offset] & TERRAIN_WATER) == 0u) return false;

    side_lengths[0] = y - origin_y;
    side_lengths[1] = size - 1 + origin_x - x;
    side_lengths[2] = origin_y - y - 1 + size;
    side_lengths[3] = x - origin_x;
    for (side = 0; side < 4; ++side) {
        const int32_t step = direction_offsets[side];
        uint32_t all_water = terrain[grid_offset + step] & TERRAIN_WATER;
        int32_t remaining = side_lengths[side] - 1;
        int32_t scan = grid_offset + step * 2;
        while (remaining > 0) {
            all_water &= terrain[scan] & TERRAIN_WATER;
            scan += step;
            --remaining;
        }
        if (all_water != 0u) {
            ++water_sides;
            if (side_lengths[side] != 0) ++nonzero_water_sides;
        } else {
            last_non_water_side = side;
        }
    }
    if (size == 2 && water_sides > 2 && nonzero_water_sides > 1 &&
        (side_lengths[last_non_water_side] == 0 || water_sides == 4))
        return true;
    if (water_sides > 3 && nonzero_water_sides > 2) return true;
    return size != 5 && water_sides == 3 && nonzero_water_sides == 3 &&
        side_lengths[last_non_water_side] == 0;
}

/* Complete C translation of Pharaoh.exe 0x004405C0. */


/* Complete logical-byte C translation of Pharaoh.exe 0x00441040. */
bool pharaoh_construction_venue_free_spots(
    const uint32_t occupancy[PHARAOH_CONSTRUCTION_VENUE_GRID_CELLS],
    int32_t extent, int32_t rows, int32_t columns,
    uint32_t *count, int32_t *locations)
{
    const int32_t available_rows = rows > 1 ? extent - rows + 1 : extent;
    const int32_t available_columns =
        columns > 1 ? extent - columns + 1 : extent;
    uint32_t found = 0;

    if (locations == NULL) return false;
    for (int32_t y = 0; y < available_columns; ++y) {
        for (int32_t x = 0; x < available_rows; ++x) {
            int empty = 1;
            for (int32_t dy = 0; dy < columns && empty; ++dy) {
                for (int32_t dx = 0; dx < rows; ++dx) {
                    if (occupancy[(x + dx) * 5 + y + dy] != 0u) {
                        empty = 0;
                        break;
                    }
                }
            }
            if (empty) {
                locations[found * 2] = x;
                locations[found * 2 + 1] = y;
                ++found;
            }
        }
    }
    *count = found;
    return true;
}

/* Complete C translation of Pharaoh.exe 0x004413B0. */
void pharaoh_construction_venue_fill_road_grid(
    uint32_t occupancy[PHARAOH_CONSTRUCTION_VENUE_GRID_CELLS],
    const uint32_t *terrain, int32_t map_grid_start,
    int32_t x, int32_t y, int32_t building_type)
{
    int32_t size;
    if (building_type == 31) size = 2;
    else if (building_type == 30) size = 3;
    else if (building_type == 33) size = 4;
    else if (building_type == 209) size = 5;
    else size = 1;

    for (int32_t i = 0; i < PHARAOH_CONSTRUCTION_VENUE_GRID_CELLS; ++i)
        occupancy[i] = 0;
    for (int32_t row = 0; row < size; ++row) {
        const int32_t grid_row = map_grid_start + (y + row) * MAP_WIDTH;
        for (int32_t column = 0; column < size; ++column) {
            occupancy[column * 5 + row] =
                (terrain[grid_row + x + column] & TERRAIN_WATER) != 0u;
        }
    }
}
