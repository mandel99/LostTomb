#ifndef RESTORE_ACTOR_CIVIC_FOLLOWUP_H
#define RESTORE_ACTOR_CIVIC_FOLLOWUP_H
#include <stdint.h>
uint32_t restore_actor_update_vagrant(void);
uint32_t restore_actor_update_ghost(void);
uint32_t restore_actor_update_explosion(void);
uint32_t restore_actor_update_trade_caravan(void);
int32_t restore_actor_advance_route_pixels(int32_t actor,int32_t steps);
void restore_actor_update_route_error(int32_t actor);
void restore_actor_step_route_pixel_y(int32_t actor);
void restore_actor_step_route_pixel_x(int32_t actor);
uint32_t restore_actor_update_engineer(void);
uint32_t restore_actor_update_fireman(void);
uint32_t restore_fireman_try_assign_fire(int32_t actor);
int32_t restore_fireman_find_nearest_fire(int32_t x,int32_t y);
uint32_t restore_actor_prepare_service_patrol(int32_t actor);
void restore_actor_advance_service_patrol(int32_t actor,int32_t steps);
int32_t restore_actor_find_remaining_delivery_destination(int32_t actor,uint32_t network);
int32_t restore_actor_apply_warehouse_delivery(int32_t delivered,int32_t actor,uint32_t network,uint8_t retire);
uint32_t restore_actor_update_warehouseman(void);
uint32_t restore_actor_update_cartpusher(void);
void restore_actor_select_cargo_image(int32_t actor);
uint32_t restore_actor_resource_cargo_image_group(uint32_t resource);
int32_t restore_actor_resource_booking_kind(int32_t kind);
int32_t restore_actor_resource_for_subtype(int32_t subtype);
void restore_actor_apply_cart_delivery(int32_t delivered,int32_t actor,uint32_t network);
#include "route_approach_candidates.h"
#include "approach_member_candidates.h"
#include "approach_search_candidates.h"
int32_t restore_actor_find_approach_kind213(uint32_t x,uint32_t y,uint32_t actor);
int32_t restore_actor_find_approach_kind222(uint32_t x,uint32_t y,uint32_t actor);
#include "acceptance_member_candidates.h"
int32_t restore_actor_find_approach_kind229(uint32_t receiver,uint32_t x,uint32_t y,int32_t actor);
int32_t restore_actor_find_approach_kind234(uint32_t receiver,uint32_t x,uint32_t y,int32_t actor);
int32_t restore_actor_find_approach_kind235(uint32_t receiver,uint32_t x,uint32_t y,int32_t actor);
int32_t restore_actor_find_approach_kind236(uint32_t receiver,uint32_t x,uint32_t y,int32_t actor);
#include "acceptance_predicates.h"
#include "worker_availability_candidates.h"
#include "worker_query_candidate.h"
int32_t FUN_00420430(int32_t building,int32_t actor);
int32_t FUN_00420910(int32_t building,int32_t actor);
int32_t FUN_00420f50(int32_t building,int32_t actor);
int32_t FUN_00421230(int32_t building,int32_t actor);
#include "base_acceptance_candidates.h"
#include "actor_route_queries.h"
#include "transport_path_candidates.h"
#endif
