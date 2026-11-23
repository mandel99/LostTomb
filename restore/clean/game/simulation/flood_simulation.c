#include "pharaoh/simulation/flood_simulation.h"

void pharaoh_flood_simulation_initialize_scale(double *simulation_scale)
{
    *simulation_scale = 9792.0 / 25.0;
}

PharaohFloodTile *pharaoh_flood_tile_initialize(
    PharaohFloodTile *tile, int32_t grid_offset, int32_t x, int32_t y,
    int32_t depth, PharaohFloodList *list, uint8_t is_floodplain)
{
    tile->grid_offset = grid_offset;
    tile->x = x;
    tile->y = y;
    tile->depth = depth;
    tile->sorted_link = 0;
    tile->list = list;
    tile->is_floodplain = is_floodplain;
    return tile;
}
