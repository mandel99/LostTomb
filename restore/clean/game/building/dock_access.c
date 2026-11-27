#include "pharaoh/building/dock_access.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohBuildingTypeRecord) == 264 &&
    offsetof(PharaohBuildingTypeRecord, house_size) == 5 &&
    offsetof(PharaohBuildingTypeRecord, farm_water_access) == 0x35,
    "shared native dock access fields");

uint32_t pharaoh_dock_is_connected_to_open_water(
    const PharaohDockAccessContext *c, int32_t x, int32_t y)
{
    (void)pharaoh_water_routing_build_distance(c->water,
        *c->river_entry_x, *c->river_entry_y);
    return pharaoh_building_has_access_neighbor(c->access, x, y, 3) != 0;
}

void pharaoh_docks_refresh_water_access(const PharaohDockAccessContext *c)
{
    (void)pharaoh_water_routing_build_distance(c->water,
        *c->river_entry_x, *c->river_entry_y);
    for (uint32_t id = 1; (int32_t)id <= *c->highest_building_id; ++id) {
        PharaohBuildingTypeRecord *b = &c->buildings[id];
        if (b->state != 1 || b->house_size != 0 ||
            (b->building_type != 75 && b->building_type != 192)) continue;
        b->farm_water_access = (uint8_t)
            (pharaoh_building_has_access_neighbor(c->access, b->x, b->y, 3) != 0);
    }
}
