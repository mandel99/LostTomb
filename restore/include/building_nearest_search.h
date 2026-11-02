#ifndef RESTORE_BUILDING_NEAREST_SEARCH_H
#define RESTORE_BUILDING_NEAREST_SEARCH_H
#include <stdint.h>
/* Logical native RET0/9-DWORD ABI; no ECX receiver. Output buffers and the
   consumed type-array entries must be valid and disjoint from map globals. */
int32_t restore_building_nearest_search(int32_t x,int32_t y,int32_t type_count,const uint32_t *types,uint32_t mask,int32_t *output_x,int32_t *output_y,int32_t rings,int32_t excluded_actor);
#endif
