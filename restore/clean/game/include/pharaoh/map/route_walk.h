#ifndef PHARAOH_MAP_ROUTE_WALK_H
#define PHARAOH_MAP_ROUTE_WALK_H

#include "pharaoh/map/route_tile_placement.h"
#include "pharaoh/map/construction_routes.h"

typedef struct PharaohRouteCoordinateDelta { int32_t x,y; } PharaohRouteCoordinateDelta;
_Static_assert(sizeof(PharaohRouteCoordinateDelta)==8 &&
    offsetof(PharaohRouteCoordinateDelta,y)==4,"native route coordinate pairs");

typedef struct PharaohRouteWalkContext {
    const PharaohRouteTilePlacementContext *tiles;
    const PharaohConstructionRouteQueries *queries;
    const int32_t *map_origin;
    const uint32_t *distances;
    uint32_t *current_tile; /* EAE000, distinct from surface BC9768. */
    const int32_t (*preferred_directions)[4]; /* 5F815C:8 rows. */
    const int32_t *tile_deltas; /* 5F80FC. */
    const PharaohRouteCoordinateDelta *coordinate_deltas; /* 5F811C. */
} PharaohRouteWalkContext;

/* Whole0051C930, physical cdecl(unused,target_x,target_y,x,y,mode).
 * First argument is never read. Full EAX0/1. Mode0 paints road,2 counts,
 * 3 paints canal; all other modes still walk the actual live distances.
 * Borrow the native priority/delta tables, not a replacement path policy. */
uint32_t pharaoh_map_place_routed_building(const PharaohRouteWalkContext *,
    int32_t target_x,int32_t target_y,int32_t x,int32_t y,int32_t mode);

#endif
