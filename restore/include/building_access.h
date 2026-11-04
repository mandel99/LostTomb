#ifndef RESTORE_BUILDING_ACCESS_H
#define RESTORE_BUILDING_ACCESS_H
#include <stdint.h>
/* 457A40: refresh access flags of kinds75/192 from the route-distance grid.
   457AF0: publish per-building access, evict disconnected households and run
   at most15 ordered repairs when the city exit remains unreachable. */
void restore_building_refresh_route_flags(void);
void restore_building_update_access(void);
uint32_t restore_building_has_access_neighbor(int32_t x,int32_t y,int32_t size);
/* Typed dispatch to reconstructed access algorithms; no original code executes. */
uint32_t restore_building_access_neighbor_query(uint32_t target,int32_t x,int32_t y,uint32_t size);
uint8_t restore_building_access_special_query(int32_t building,uint32_t *tile,int32_t *x,int32_t *y);
void restore_building_access_remove_disconnected(void);
#endif
