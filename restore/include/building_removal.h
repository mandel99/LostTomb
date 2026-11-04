#ifndef RESTORE_BUILDING_REMOVAL_H
#define RESTORE_BUILDING_REMOVAL_H
#include <stdint.h>
void restore_building_clear_four_parts(int32_t building);
void restore_building_clear_linked_parts(int32_t building,int32_t destroy);
void restore_building_clear_farm_tiles(int32_t building,int32_t kind);
void restore_building_spawn_removal_effects(int32_t x,int32_t y,uint32_t size,uint32_t expanded);
void FUN_004c03d0(int32_t building);
void FUN_004695d0(int32_t building,int32_t destroy);
void FUN_0043fd00(int32_t building,int32_t kind);
void FUN_0048c0e0(int32_t x,int32_t y,uint32_t size,uint32_t expanded);
#endif
