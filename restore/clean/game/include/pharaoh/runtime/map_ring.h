#ifndef PHARAOH_RUNTIME_MAP_RING_H
#define PHARAOH_RUNTIME_MAP_RING_H

#include "pharaoh/map/ring_types.h"

void pharaoh_map_ring_generate_tiles(PharaohRingOffsetTables *tables);
void pharaoh_map_ring_initialize(const PharaohMapRingInitContext *context);

#endif
