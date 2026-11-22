#ifndef PHARAOH_MAP_ACCESS_ROUTING_H
#define PHARAOH_MAP_ACCESS_ROUTING_H

#include "pharaoh/map/routing_search.h"

typedef struct PharaohAccessRepairMap {
    PharaohRoutingSearchState *routing;
    uint32_t *terrain;
    uint8_t *edges, *footprints;
} PharaohAccessRepairMap;

/* Native00517A40/00517C20. Routing arrays include the native border cells. */
uint8_t pharaoh_map_routing_citizen_access(PharaohRoutingSearchState *state,
    int32_t x, int32_t y, int32_t target_x, int32_t target_y);
uint32_t pharaoh_map_routing_clear_first_blocker(PharaohAccessRepairMap *map,
    int32_t x, int32_t y, int32_t target_x, int32_t target_y);

#endif
