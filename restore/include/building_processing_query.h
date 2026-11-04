#ifndef RESTORE_BUILDING_PROCESSING_QUERY_H
#define RESTORE_BUILDING_PROCESSING_QUERY_H
#include <stdint.h>
uint32_t restore_kind_is_processing_type(int32_t kind);
uint32_t restore_building_is_processing_type(uint32_t building);
uint32_t restore_building_resource_is_permitted(int32_t building);
uint32_t restore_city_has_unpermitted_occupied_building(uint32_t self);
uint32_t FUN_00487f80(int32_t kind);
uint32_t FUN_00459240(int32_t building);
/* Explicit C receiver; the actor-message frame carries native ECX. */
uint32_t FUN_004bf9d0(uint32_t self);
#endif
