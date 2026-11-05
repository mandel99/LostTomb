#ifndef RESTORE_ROUTE_APPROACH_CANDIDATES_H
#define RESTORE_ROUTE_APPROACH_CANDIDATES_H
#include <stdint.h>
void restore_actor_choose_nearby_road_direction(int32_t actor);
int32_t restore_actor_find_linked_kind85(int32_t actor, int32_t *length);
uint32_t restore_actor_approach_destination_result(uint32_t x, uint32_t y, int32_t actor);
int32_t restore_actor_find_approach_building_state2(uint32_t x, uint32_t y, int32_t actor);
int32_t restore_actor_find_approach_building_state3(uint32_t x, uint32_t y, int32_t actor);
void restore_actor_follow_leader_closely(int32_t actor, int32_t leader, uint32_t mode, int32_t distance);
#endif
