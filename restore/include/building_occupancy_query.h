#ifndef RESTORE_BUILDING_OCCUPANCY_QUERY_H
#define RESTORE_BUILDING_OCCUPANCY_QUERY_H
#include <stdint.h>
/* Full native EAX retained: true keeps tile high24 bits; false keeps height
   high24 bits. Only AL is the predicate. Typed C aliases use explicit cdecl
   arguments; native RET4/RET16 are retained by the legacy frame adapters. */
uint32_t restore_region_has_blocking_tile(int32_t x,int32_t y,int32_t width,int32_t height,uint32_t mask);
uint32_t restore_region_has_active_occupant(int32_t x,int32_t y,int32_t width,int32_t height);
uint32_t restore_building_has_active_occupant(int32_t building);
uint32_t FUN_004893c0(int32_t x,int32_t y,int32_t width,int32_t height,uint32_t mask);
uint32_t FUN_00489470(int32_t x,int32_t y,int32_t width,int32_t height);
uint32_t FUN_004894a0(int32_t building);
#endif
