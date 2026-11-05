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
#endif
