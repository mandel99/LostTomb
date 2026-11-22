#ifndef PHARAOH_MAP_ROAD_NETWORK_H
#define PHARAOH_MAP_ROAD_NETWORK_H

#include "pharaoh/scenario/saved_game_initialize.h"
#include "pharaoh/map/road_network_types.h"

enum { PHARAOH_ROAD_GRID_TILES = 51984, PHARAOH_ROAD_QUEUE_SIZE = 1000 };
typedef struct PharaohRoadNetworkContext {
    const PharaohSavedGameGeometry *geometry;
    PharaohScenarioPlayerRuntime *players;
    const int32_t *current_player;
    const uint32_t *terrain;
    const int8_t *citizen_routing;
    uint8_t *regions, *shades;
    uint32_t region_size, shade_steps, shade;
    uint32_t queue_head, queue_tail, scan_tile;
    uint32_t queue[PHARAOH_ROAD_QUEUE_SIZE];
} PharaohRoadNetworkContext;

void pharaoh_map_color_road_region(PharaohRoadNetworkContext *c,
    uint32_t tile, uint8_t region);
void pharaoh_city_rebuild_road_networks(PharaohRoadNetworkContext *c);

#endif
