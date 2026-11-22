#ifndef PHARAOH_MAP_SURFACE_QUERIES_H
#define PHARAOH_MAP_SURFACE_QUERIES_H
#include <stdint.h>
uint8_t pharaoh_map_sweep_has_passed(int32_t countdown,int32_t sweep_start,int32_t column);
int32_t pharaoh_map_road_canal_crossing_image(uint32_t unused_grid_offset,int32_t image_offset);
#endif
