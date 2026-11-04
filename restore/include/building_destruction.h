#ifndef RESTORE_BUILDING_DESTRUCTION_H
#define RESTORE_BUILDING_DESTRUCTION_H
#include <stdint.h>
void restore_building_destroy(int32_t building,uint32_t mode);
void restore_building_destroy_four_parts(int32_t building);
uint32_t restore_building_destruction_root(uint32_t building);
void restore_building_clear_four_part_occupancy(int32_t building);
void restore_population_remove_destroyed(int32_t player,int32_t people);
void FUN_0046d100(int32_t building,uint32_t mode);
void FUN_004c04a0(int32_t building);
uint32_t FUN_0046d0f0(uint32_t building);
void FUN_004bfee0(int32_t building);
void FUN_004e9170(int32_t player,int32_t people);
#endif
