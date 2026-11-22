#ifndef PHARAOH_MAP_ROUTING_SEARCH_H
#define PHARAOH_MAP_ROUTING_SEARCH_H

#include <stdint.h>

enum {
    PHARAOH_ROUTING_GRID_WIDTH = 228,
    PHARAOH_ROUTING_GRID_CELLS = 0xcb10
};

typedef int32_t (*PharaohRoutingTileQuery)(void *user_data,
    int32_t grid_offset);

typedef struct PharaohRoutingSearchState {
    int32_t *distance;
    int32_t *queue_items;
    const int8_t *land_citizen;
    const uint32_t *terrain;
    int32_t grid_start;
    int32_t source_offset;
    int32_t queue_head;
    int32_t queue_tail;
    int32_t total_routes_calculated;
    PharaohRoutingTileQuery no_fighting_friendly;
    void *user_data;
} PharaohRoutingSearchState;

int32_t pharaoh_map_routing_citizen_find_first_of_any(
    PharaohRoutingSearchState *state, int32_t source_x, int32_t source_y,
    const int32_t *target_offsets, int32_t target_count);

#endif
