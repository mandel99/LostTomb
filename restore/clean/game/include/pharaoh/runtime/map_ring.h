#ifndef PHARAOH_RUNTIME_MAP_RING_H
#define PHARAOH_RUNTIME_MAP_RING_H

#include "pharaoh/map/ring_types.h"

void pharaoh_map_ring_generate_tiles(PharaohRingOffsetTables *tables);
void pharaoh_map_ring_initialize(const PharaohMapRingInitContext *context);

uint8_t pharaoh_map_ring_point_is_outside(const PharaohRingOffsetTables *tables,
    int32_t width,int32_t height,int32_t x,int32_t y,int32_t family,int32_t ring,int32_t point);

#endif
