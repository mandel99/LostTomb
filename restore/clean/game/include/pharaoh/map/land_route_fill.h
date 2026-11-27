#ifndef PHARAOH_MAP_LAND_ROUTE_FILL_H
#define PHARAOH_MAP_LAND_ROUTE_FILL_H

#include "pharaoh/map/construction_routes.h"
#include "pharaoh/map/grid_geometry.h"

_Static_assert(sizeof(PharaohBuildingTypeRecord)==264 &&
    offsetof(PharaohBuildingTypeRecord,building_type)==16,
    "native construction building stride and signed WORD type");

typedef struct PharaohLandRouteFillContext {
    const PharaohConstructionRouteContext *route;
    const PharaohMapGridGeometry *geometry;
    const int32_t *world_x, *world_y; /* BCA2E8/BCA2EC. */
} PharaohLandRouteFillContext;

/* Whole native0051BC60. Physical cdecl(unused,x,y,mode); the unused first
 * stack argument is never read. Full EAX0/1. The checked neighbor bounds
 * use signed absolute tile%228,tile/228, without subtracting map_origin.
 * Full native bordered grids and real building/flood objects are required. */
uint32_t pharaoh_map_construction_checked_distances(
    const PharaohLandRouteFillContext *,int32_t x,int32_t y,int32_t mode);

#endif
