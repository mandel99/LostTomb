#ifndef RESTORE_BUILDING_ACCESS_SEARCH_H
#define RESTORE_BUILDING_ACCESS_SEARCH_H
#include <stdint.h>
uint32_t restore_building_find_access(int32_t x,int32_t y,uint32_t size);
uint32_t restore_building_find_wide_access(int32_t x,int32_t y,uint32_t size);
uint32_t restore_building_fixed_entry(int32_t x,int32_t y);
uint32_t restore_building_special_entry(int32_t building,uint32_t *tile,int32_t *x,int32_t *y);
void restore_building_remove_highest_serial(void);
/* Existing typed C child boundaries used by the removal controller. */
void restore_building_access_remove_special(int32_t building);
void restore_building_access_refresh_terrain(int32_t x,int32_t y,uint32_t size);
uint32_t FUN_00481060(int32_t x,int32_t y,uint32_t size);
uint32_t FUN_00481210(int32_t x,int32_t y,uint32_t size);
uint32_t FUN_00487d00(int32_t x,int32_t y,uint32_t unused);
uint32_t FUN_004f30b0(int32_t building,uint32_t *tile,int32_t *x,int32_t *y);
void FUN_00458270(void);
#endif
