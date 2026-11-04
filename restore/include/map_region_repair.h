#ifndef RESTORE_MAP_REGION_REPAIR_H
#define RESTORE_MAP_REGION_REPAIR_H
#include <stdint.h>
void restore_map_refresh_paved_region(int32_t left,int32_t top,int32_t right,int32_t bottom,uint32_t clear);
void restore_map_refresh_channel_region(int32_t left,int32_t top,int32_t right,int32_t bottom,uint32_t refresh);
#endif
