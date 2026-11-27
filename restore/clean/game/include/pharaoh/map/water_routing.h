#ifndef PHARAOH_MAP_WATER_ROUTING_H
#define PHARAOH_MAP_WATER_ROUTING_H

#include <stdint.h>

typedef struct PharaohWaterRoutingContext {
    const uint8_t *routing_tiles; /* ED4140, 51984 bytes. */
    uint32_t *distance; /* EED760, 51984 DWORDs; shared with access queries. */
    uint8_t *deferred_visits; /* EAE010, 51984 bytes, DWORD-aligned. */
    uint32_t *queue; /* F203A0, 51984 DWORDs. */
    uint32_t *start, *queue_head, *queue_tail; /* EAE004/00C/008. */
    const int32_t *origin; /* BCA2D8. */
} PharaohWaterRoutingContext;

/* Complete native0051CE90. Fixed228 pitch, native sentinel-bordered backing
 * grids and queue; no additional clipping or queue-full policy. Full EAX0/1. */
uint32_t pharaoh_water_routing_build_distance(
    const PharaohWaterRoutingContext *, int32_t x, int32_t y);

#endif
