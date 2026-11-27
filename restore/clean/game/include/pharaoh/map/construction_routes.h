#ifndef PHARAOH_MAP_CONSTRUCTION_ROUTES_H
#define PHARAOH_MAP_CONSTRUCTION_ROUTES_H
#include "pharaoh/map/terrain_queries.h"
#include "pharaoh/simulation/flood_simulation.h"

typedef struct PharaohConstructionRouteQueries {
    const uint32_t *terrain;
    const int32_t *image_ids,*orientation;
    const int16_t *building_ids;
    const PharaohBuildingTypeRecord *buildings;
    PharaohFloodTile *const *flood_tiles;
    const int8_t *height_values; /*AC2D4C*/
    const uint8_t *property_flags; /*B285CC*/
    const uint32_t *distances; /*same EED760 storage as distance builder*/
    uint32_t *road_flags; /*BC97C0,eight DWORDs; only cardinal slots changed*/
    const int32_t *group_f56b84,*group_f56c34,*group_f56cd4;
    const int32_t *group_f5695c,*group_f56c44,*group_f56c94;
} PharaohConstructionRouteQueries;

typedef struct PharaohConstructionRouteContext {
    const PharaohConstructionRouteQueries *queries;
    const int32_t *map_origin;
    const uint8_t *routing; /*EE0C50,current native routing-cost grid*/
    uint32_t *distances,*queue; /*51984 cells,EED760/F203A0*/
    uint8_t *marks; /*51984 bytes,EAE010*/
    uint32_t *start,*head,*tail,*calls; /*EAE004/00C/008,EADFD4*/
} PharaohConstructionRouteContext;

/* Complete0047E0D0/0047E090/0051C510/0051C630 and0051B680/0051C790.
   Real shared bordered maps, pointer objects, lookup groups, scratch and queue
   required. No invented border/ID/cycle limits. Crossing exposes logical AL. */
uint32_t pharaoh_map_adjacent_road_count(const PharaohConstructionRouteQueries *,uint32_t);
uint8_t pharaoh_map_road_has_crossing(const uint32_t *terrain,int32_t tile);
uint32_t pharaoh_map_can_place_road_on_canal_object(
    const PharaohConstructionRouteQueries *,uint32_t);
uint32_t pharaoh_map_can_place_canal_on_road(
    const PharaohConstructionRouteQueries *,uint32_t);
uint32_t pharaoh_map_construction_distances(
    const PharaohConstructionRouteContext *,int32_t x,int32_t y,int32_t mode);
uint32_t pharaoh_map_construction_wall_distances(
    const PharaohConstructionRouteContext *,int32_t x,int32_t y);

struct PharaohFerryMapContext;
/* Bind the same native EED760/F203A0/EAE004/008/00C and BC97C0 owners
   already used by ferries. Other world fields/marks/counter remain caller
   inputs; no replacement buffers are allocated or zero defaults installed. */
void pharaoh_construction_routes_bind_shared(PharaohConstructionRouteContext *,
    PharaohConstructionRouteQueries *,const struct PharaohFerryMapContext *);
/* First-image view of the actual loaded native eight-byte sprite groups. */
void pharaoh_construction_routes_bind_groups(PharaohConstructionRouteQueries *,
    const int32_t *image_group_first);
#endif
