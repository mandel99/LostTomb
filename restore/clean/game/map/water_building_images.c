#include "pharaoh/map/water_building_images.h"
#include "pharaoh/map/surface_queries.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohMapSquareCell)==8,
    "native footprint matrix cell stride");

/* Complete native47CF00/47D1F0 and487DC0/487E30; existing clean C moved
 * from terrain_queries.c, reused from prior BuildingFootprint C. */
static void map_building_tiles_add_water(PharaohBuildingFootprintMap *map,
    int32_t size, int16_t building_id, int32_t x, int32_t y,
    uint32_t image_id)
{
    int32_t access_x, access_y, row;
    if (map->view_orientation == 0) {
        access_x = 0;
        access_y = size - 1;
    } else if (map->view_orientation == 2) {
        access_x = 0;
        access_y = 0;
    } else if (map->view_orientation == 4) {
        access_x = size - 1;
        access_y = 0;
    } else {
        access_x = size - 1;
        access_y = size - 1;
    }
    if (x < 0 || y < 0 || x + size - 1 >= map->width ||
        y + size - 1 >= map->height) {
        *map->placement_error = 1;
        return;
    }
    *map->cursor_offset = map->origin + y * PHARAOH_MAP_GRID_WIDTH + x;
    for (row = 0; row < size; ++row) {
        int32_t column;
        for (column = 0; column < size; ++column) {
            const PharaohMapSquareCell *cell = &map->square_cells[row][column];
            const int32_t tile = *map->cursor_offset + cell->grid_offset;
            const uint32_t old_terrain = map->terrain[tile];
            map->terrain[tile] = old_terrain | 8u;
            if ((old_terrain & 4u) == 0)
                map->terrain[tile] = (old_terrain & 0x03052e80u) | 8u;
            map->building_ids[tile] = building_id;
            if (size == 2)
                map->property_flags[tile] =
                    (map->property_flags[tile] & 0xe1u) | 1u;
            else
                map->property_flags[tile] =
                    (map->property_flags[tile] & 0xefu) | 2u;
            map->image_ids[tile] = image_id;
            map->tile_states[tile] = cell->image_offset;
            if (column == access_x && row == access_y)
                map->tile_states[tile] |= 0x40u;
        }
    }
}

void pharaoh_map_building_tiles_add_water_size2(
    PharaohBuildingFootprintMap *map, int16_t building_id,
    int32_t x, int32_t y, uint32_t image_id)
{
    map_building_tiles_add_water(map, 2, building_id, x, y, image_id);
}

void pharaoh_map_building_tiles_add_water_size3(
    PharaohBuildingFootprintMap *map, int16_t building_id,
    int32_t x, int32_t y, uint32_t image_id)
{
    map_building_tiles_add_water(map, 3, building_id, x, y, image_id);
}

int32_t pharaoh_map_terrain_2x2_is_flooded(
    PharaohFloodTile *const *flood_tiles, int32_t map_origin,
    int32_t x, int32_t y)
{
    const int32_t tile = map_origin + y * PHARAOH_MAP_GRID_WIDTH + x;
    const PharaohFloodTile *flood = flood_tiles[tile];
    if (flood != 0 && flood->is_floodplain == 0) return 1;
    flood = flood_tiles[tile + 1];
    if (flood != 0 && flood->is_floodplain == 0) return 1;
    flood = flood_tiles[tile + PHARAOH_MAP_GRID_WIDTH];
    if (flood != 0 && flood->is_floodplain == 0) return 1;
    flood = flood_tiles[tile + PHARAOH_MAP_GRID_WIDTH + 1];
    return flood != 0 && flood->is_floodplain == 0;
}

int32_t pharaoh_map_building_is_flooded(
    PharaohFloodTile *const *flood_tiles, int32_t map_origin,
    const PharaohBuildingTypeRecord *buildings, int32_t building_id)
{
    return pharaoh_map_terrain_2x2_is_flooded(flood_tiles, map_origin,
        buildings[building_id].x, buildings[building_id].y);
}

/* Typed view of the existing shared footprint matrix and map storage. */
void pharaoh_map_bind_water_footprint(PharaohBuildingFootprintMap *map,
    PharaohMapSurfaceContext *surface)
{
    *map=(PharaohBuildingFootprintMap){surface->map->terrain,surface->building_ids,
        surface->property_flags,surface->image_ids,surface->edge_flags,
        surface->map->origin,surface->map->width,surface->map->height,
        *surface->building_orientation,(int32_t *)(void *)surface->placement_failed,
        (int32_t *)(void *)surface->current_tile,
        (const PharaohMapSquareCell (*)[6])(const void *)surface->footprints};
}

/* Complete native00487EC0, reused prior BuildingFootprint C. */
void pharaoh_map_update_water_lift_image(PharaohMapSurfaceContext *surface,
    PharaohBuildingFootprintMap *map,PharaohWaterLiftImageState *state,
    int32_t building)
{
    state->uses_variant=(uint8_t)pharaoh_map_building_is_flooded(
        surface->flood_tiles,surface->map->origin,surface->buildings,building);
    state->variant_high=(uint8_t)(surface->buildings[building].farm_water_access!=0);
    int32_t direction=(int32_t)((uint32_t)surface->buildings[building].reserved_aa[0]-
        (uint32_t)(*surface->orientation/2));
    if (direction<0) direction=(int32_t)((uint32_t)direction+4u);
    const uint8_t alternate=pharaoh_map_sweep_has_passed(*surface->sweep_countdown,
        *surface->sweep_start,surface->buildings[building].x);
    static const uint16_t primary[4]={547,537,536,535};
    for (uint32_t i=0;i<4;++i) {
        state->primary_images[i]=(int32_t)surface->groups[primary[i]].first_image;
        state->alternate_images[i]=(int32_t)surface->groups[730+i].first_image;
    }
    const uint32_t image=(uint32_t)pharaoh_map_water_lift_image(state,direction,alternate);
    pharaoh_map_bind_water_footprint(map,surface);
    pharaoh_map_building_tiles_add_water_size2(map,(int16_t)building,
        surface->buildings[building].x,surface->buildings[building].y,image);
}

/* Complete native00487E60. Both intake deltas belong to the raw BYTE view. */
uint32_t pharaoh_map_water_lift_has_intake(const uint32_t *terrain,
    const PharaohBuildingTypeRecord *buildings,const int32_t offsets[8],
    int32_t building)
{
    const PharaohBuildingTypeRecord *const b=&buildings[building];
    const uint32_t orientation=b->reserved_aa[0];
    const uint32_t tile=(uint32_t)b->grid_offset;
    if ((terrain[tile+(uint32_t)offsets[orientation]]&4u)!=0) return 1;
    return (terrain[tile+(uint32_t)offsets[orientation+4u]]&4u)!=0;
}
