#ifndef RESTORE_CITY_REGION_CHILDREN_H
#define RESTORE_CITY_REGION_CHILDREN_H
#include <stdint.h>
/* Logical memory effects; original RAW models retain incidental registers. */
void restore_city_color_road_region(uint32_t tile,uint32_t region);
void restore_city_mark_linked_fire(uint32_t building);
void FUN_004689e0(int32_t tile,uint8_t region);
void FUN_00468b20(uint32_t building);
#endif
