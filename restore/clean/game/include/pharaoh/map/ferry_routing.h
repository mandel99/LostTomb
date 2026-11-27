#ifndef PHARAOH_MAP_FERRY_ROUTING_H
#define PHARAOH_MAP_FERRY_ROUTING_H
#include "pharaoh/building/type_queries.h"
#include "pharaoh/core/ring_queue.h"

typedef struct PharaohFerryMapContext {
    const uint32_t *origin,*orientation;
    uint32_t *terrain;
    const uint8_t *water_cost,*tile_random;
    uint8_t *land_routing;
    const uint32_t (*shore_patterns)[4];
    const int32_t *corner_offsets,*ring_offsets;
    const int32_t (*ring_xy)[2];
    const int32_t *direction_offsets;
    uint32_t *current_tile,*adjacent_flags;
    int32_t *result_x,*result_y;
    uint32_t *distances,*queue; /*51984 DWORDs each*/
    uint32_t *active_tile,*queue_head,*queue_tail;
    const uint32_t *random_second_7; /*D3B374*/
    uint8_t *temporary_path,*route_slots; /*500 and1000x500 bytes*/
} PharaohFerryMapContext;

typedef struct PharaohFerryContext {
    PharaohBuildingTypeRecord *buildings;
    PharaohWalkerQueryRecord *walkers;
    int16_t *slot_walkers;
    PharaohFerryQueueRecord *queues;
    PharaohFerryMapContext *map;
} PharaohFerryContext;

/* Complete0047CBD0/004806C0/0051DE70/0051E720. Native fixed arrays,
   lookup tables, border cells and shared scratch must all be genuine. */
uint32_t pharaoh_map_shore_pattern(const PharaohFerryMapContext *,int32_t x,int32_t y);
uint32_t pharaoh_map_water_tile_next_to(PharaohFerryMapContext *,int32_t x,int32_t y,uint32_t size);
uint32_t pharaoh_map_ferry_water_distances(PharaohFerryMapContext *,int32_t x,int32_t y,int32_t end_x,int32_t end_y);
int32_t pharaoh_map_water_path_4dir(PharaohFerryMapContext *,int32_t route,int32_t target_x,int32_t target_y,int32_t x,int32_t y,int32_t variation,uint32_t *output);
/* Complete0043F060/004A3630/004A36B0. */
void pharaoh_ferry_update_tiles(PharaohFerryContext *,int32_t building,uint8_t enabled,int32_t marker);
void pharaoh_ferry_queue_release(PharaohFerryContext *,int32_t building);
void pharaoh_ferry_queues_release(PharaohFerryContext *,int32_t building);
typedef struct PharaohSceneRouting PharaohSceneRouting;
/* Bind actual C implementations to existing scene-routing callbacks. */
void pharaoh_ferry_bind_scene_routing(PharaohSceneRouting *,PharaohFerryContext *);
#endif
