#ifndef PHARAOH_MAP_ROAD_ACCESS_H
#define PHARAOH_MAP_ROAD_ACCESS_H

#include "pharaoh/building/type_queries.h"
#include "pharaoh/map/road_network_types.h"

typedef struct PharaohRoadAccessContext {
    int32_t grid_start;
    const uint32_t *terrain;
    const int16_t *building_ids;
    const PharaohBuildingTypeRecord *buildings;
    const uint8_t *regions;
    const PharaohRoadNetworkRank *ranks;
    const int32_t *ring_offsets, *ring_xy;
    int32_t result_x, result_y;
} PharaohRoadAccessContext;

int32_t pharaoh_map_has_road_access(PharaohRoadAccessContext *c,
    int32_t x, int32_t y, int32_t size);
int32_t pharaoh_map_has_road_access_nearby(PharaohRoadAccessContext *c,
    int32_t x, int32_t y);
int32_t pharaoh_map_has_road_access_temple_complex(PharaohRoadAccessContext *c,
    int32_t x, int32_t y, int32_t orientation);

#endif
