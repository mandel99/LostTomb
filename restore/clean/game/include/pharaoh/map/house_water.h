#ifndef PHARAOH_MAP_HOUSE_WATER_H
#define PHARAOH_MAP_HOUSE_WATER_H

#include "pharaoh/building/type_queries.h"

typedef struct PharaohMapHouseWaterContext {
    const PharaohBuildingTypeRecord *buildings;
    const int16_t *building_ids;
    int32_t map_width, map_height, map_origin;
    int32_t *result_x, *result_y;
} PharaohMapHouseWaterContext;

/* Complete native0047FD30 owner. Returns0 when a nearby occupied house lacks
 * water,1 when at least one nearby occupied house has water, and2 when none
 * is present. The result coordinates retain the native scan side effects. */
int32_t pharaoh_map_houses_water_in_radius(
    const PharaohMapHouseWaterContext *context,
    int32_t building_id, int32_t radius);

#endif
