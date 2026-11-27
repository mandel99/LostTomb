#ifndef PHARAOH_MAP_CONSTRUCTION_CONNECTIONS_H
#define PHARAOH_MAP_CONSTRUCTION_CONNECTIONS_H
#include "pharaoh/building/construction_undo.h"
#include "pharaoh/building/model_types.h"
#include "pharaoh/map/construction_footprint.h"
#include "pharaoh/map/route_walk.h"
#include "pharaoh/map/ferry_routing.h"

typedef struct PharaohConstructionConnectionContext {
    PharaohConstructionFootprintContext *footprint;
    const PharaohConstructionUndoContext *backup;
    const PharaohConstructionRouteContext *routes;
    const PharaohRouteWalkContext *walk;
    const PharaohBuildingModelRecord *models;
    const PharaohMapTileAreaOffset (*footprints)[6]; /* Native5CC8C0. */
    const int32_t (*endpoints)[2]; /* Native5DC188,4 pairs. */
    uint8_t *cleared; /* AE8E7C. */
    const uint32_t *drag_started; /* 6059E4. */
    int32_t *start_status,*end_status; /* BC970C/BC9708. */
    uint32_t *delta_x,*delta_y,*preview_cost; /* D3B4F8/FC,BC9724. */
} PharaohConstructionConnectionContext;

/* Whole0047CA50 and0047EDA0. Borrow the actual EED760 distance grid,
 * Native lookup tables and common backup/surface/cost/count state. */
void pharaoh_construction_clear_route_footprint(
    const PharaohConstructionConnectionContext *,uint32_t mode,
    int32_t x,int32_t y,int32_t width,int32_t height);
uint32_t pharaoh_construction_plan_connection(
    const PharaohConstructionConnectionContext *,uint32_t preview,
    int32_t x1,int32_t y1,int32_t x2,int32_t y2);

typedef struct PharaohFerryRouteQueryContext {
    PharaohFerryMapContext *map;
    const int32_t *ferry_state,*radius; /* BCA310/BC96E4. */
} PharaohFerryRouteQueryContext;

/* Whole00486A30. Count is written only after both genuine shore queries
 * succeed. Native orientation/radius and common scratch stay live. */
uint32_t pharaoh_construction_ferry_route_is_valid(
    const PharaohFerryRouteQueryContext *,int32_t x1,int32_t y1,
    int32_t x2,int32_t y2,uint32_t layout,int32_t orientation,
    int32_t wanted_orientation,uint32_t *path,int32_t *count);
#endif
