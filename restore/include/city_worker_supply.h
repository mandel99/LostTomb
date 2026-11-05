#ifndef RESTORE_CITY_WORKER_SUPPLY_H
#define RESTORE_CITY_WORKER_SUPPLY_H
#include <stdint.h>
/* Logical contracts; original RAW providers retain physical registers/flags. */
void restore_city_reset_dispatch_reservations(int32_t building);
void restore_city_refresh_nearby_house_needs(int32_t building,int32_t radius);
int32_t restore_city_find_vacant_service_building(uint32_t actor_kind);
void restore_actor_initialize_type_parameters(int32_t actor);
void restore_city_dispatch_kind85(int32_t building);
#endif
