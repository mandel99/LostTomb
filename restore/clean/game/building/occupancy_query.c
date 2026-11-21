#include "pharaoh/building/occupancy_query.h"

uint32_t pharaoh_region_has_blocking_tile(PharaohOccupancyState *state,
    int32_t x, int32_t y, int32_t width, int32_t height, uint32_t mask)
{
    const uint32_t origin = state->grid_origin + (uint32_t)y * 228u +
        (uint32_t)x;
    uint32_t row;
    state->last_origin = origin;
    if (height > 0 && width > 0) {
        for (row = 0; (int32_t)row < height; ++row) {
            uint32_t column;
            for (column = 0; (int32_t)column < width; ++column) {
                const uint32_t tile = origin +
                    state->offsets[row * 6u + column].offset;
                if ((mask & 0x10000u) != 0u && state->objects[tile] != 0 &&
                    state->objects[tile][24] == 0u) {
                    return (tile & 0xffffff00u) | 1u;
                }
                if ((state->terrain[tile] & mask) != 0u)
                    return (tile & 0xffffff00u) | 1u;
            }
        }
    }
    return (uint32_t)height & 0xffffff00u;
}

uint32_t pharaoh_region_has_active_occupant(PharaohOccupancyState *state,
    int32_t x, int32_t y, int32_t width, int32_t height)
{
    return pharaoh_region_has_blocking_tile(state, x, y, width, height,
        0x10000u);
}

uint32_t pharaoh_building_has_active_occupant(PharaohOccupancyState *state,
    int32_t building)
{
    const PharaohOccupancyBuilding *record = &state->buildings[building];
    return pharaoh_region_has_active_occupant(state,
        record->x, record->y, 3, 3);
}
