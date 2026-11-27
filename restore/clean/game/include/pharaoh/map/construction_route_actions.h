#ifndef PHARAOH_MAP_CONSTRUCTION_ROUTE_ACTIONS_H
#define PHARAOH_MAP_CONSTRUCTION_ROUTE_ACTIONS_H

#include "pharaoh/building/construction_undo.h"
#include "pharaoh/map/land_route_fill.h"
#include "pharaoh/map/route_walk.h"
#include "pharaoh/scenario/scene_routing.h"

typedef struct PharaohConstructionRouteActionsContext {
    const PharaohLandRouteFillContext *fill;
    const PharaohRouteWalkContext *walk;
    const PharaohConstructionUndoContext *backup;
    uint8_t *cleared; /* Actual mutable AE8E7C grid, backup saved_bytes[0]. */
    PharaohSceneRouting *routing;
    uint8_t *map_changed; /* E38DCA. */
} PharaohConstructionRouteActionsContext;

/* Whole00474F50/00475090, physical cdecl(preview,x1,y1,x2,y2), logical void.
 * Restore the existing terrain/cleared/image backup before checking sites.
 * Preview still performs the genuine route painting; only the later route
 * refresh/changed publication is skipped. All contexts share the same live
 * map, building pool, image layers, origin and routing scratch. The citizen
 * child requires real ferry providers when it visits active ferry records. */
void pharaoh_construction_place_road_route(
    const PharaohConstructionRouteActionsContext *,int32_t preview,
    int32_t x1,int32_t y1,int32_t x2,int32_t y2);
void pharaoh_construction_place_canal_route(
    const PharaohConstructionRouteActionsContext *,int32_t preview,
    int32_t x1,int32_t y1,int32_t x2,int32_t y2);

#endif
