#include "pharaoh/map/moisture.h"
#include "pharaoh/map/grid_geometry.h"

static int map_object_blocks(PharaohFloodTile *const *objects, int32_t tile)
{
    return objects[tile] != NULL && objects[tile]->is_floodplain == 0;
}

uint8_t pharaoh_map_moisture_flood_objects_around(
    PharaohFloodTile *const *objects, int32_t tile)
{
    if (map_object_blocks(objects, tile - PHARAOH_MAP_GRID_WIDTH) &&
        map_object_blocks(objects, tile + PHARAOH_MAP_GRID_WIDTH)) return 1;
    return map_object_blocks(objects, tile - 1) &&
        map_object_blocks(objects, tile + 1);
}

uint8_t pharaoh_map_opposite_pair_has_mask(
    const uint32_t *terrain, int32_t offset, uint32_t mask)
{
    if ((terrain[offset - PHARAOH_MAP_GRID_WIDTH] & mask) != 0 &&
        (terrain[offset + PHARAOH_MAP_GRID_WIDTH] & mask) != 0) return 1;
    return (terrain[offset - 1] & mask) != 0 &&
        (terrain[offset + 1] & mask) != 0;
}

uint32_t pharaoh_map_moisture_or_wet(const uint32_t *terrain,
    PharaohFloodTile *const *objects, const uint8_t *moisture, int32_t tile)
{
    if (((terrain[tile - PHARAOH_MAP_GRID_WIDTH] & 4u) != 0 &&
         (terrain[tile + PHARAOH_MAP_GRID_WIDTH] & 4u) != 0) ||
        ((terrain[tile - 1] & 4u) != 0 &&
         (terrain[tile + 1] & 4u) != 0) ||
        pharaoh_map_moisture_flood_objects_around(objects, tile)) return 100;
    return moisture[tile];
}

uint32_t pharaoh_map_moisture_neighbour_shade(const uint32_t *terrain,
    PharaohFloodTile *const *objects, const uint8_t *moisture,
    int32_t tile, int32_t offset)
{
    const int32_t target = (int32_t)((uint32_t)tile + (uint32_t)offset);
    const uint32_t value =
        pharaoh_map_moisture_or_wet(terrain, objects, moisture, target);
    if (map_object_blocks(objects, target) ||
        (terrain[target] & 0x40004u) != 0) return 100;
    return value < 128u ? value : 0u;
}
