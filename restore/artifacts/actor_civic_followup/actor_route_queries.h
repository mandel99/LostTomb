#ifndef RESTORE_ACTOR_ROUTE_QUERIES_H
#define RESTORE_ACTOR_ROUTE_QUERIES_H
#include <stdint.h>

uint32_t restore_actor_prune_service_road_exits(int32_t tile,uint32_t actor);
uint32_t restore_actor_flood_unblocked_terrain(int32_t x,int32_t y);
uint32_t restore_actor_search_accessible_terrain(int32_t x,int32_t y,int32_t target_x,int32_t target_y);
int32_t restore_actor_search_accessible_candidates(int32_t x,int32_t y,const int32_t *candidate_tiles,int32_t candidate_count);
int32_t restore_actor_build_straight_gradient_route(int32_t slot,int32_t target_x,int32_t target_y,int32_t x,int32_t y,int32_t straight_steps);
int32_t restore_actor_build_terrain_gradient_route(int32_t slot,int32_t target_x,int32_t target_y,int32_t x,int32_t y,int32_t use_tile_preference);

uint32_t FUN_0047e340(int32_t tile,uint32_t actor);
uint32_t FUN_0051d160(int32_t x,int32_t y);
uint32_t FUN_0051d420(int32_t x,int32_t y,int32_t target_x,int32_t target_y);
int32_t FUN_0051d6d0(int32_t x,int32_t y,int32_t candidates,int32_t count);
int32_t FUN_0051d980(int32_t slot,int32_t target_x,int32_t target_y,int32_t x,int32_t y,int32_t straight_steps);
int32_t FUN_0051dc10(int32_t slot,int32_t target_x,int32_t target_y,int32_t x,int32_t y,int32_t use_tile_preference);
#endif
