#ifndef PHARAOH_MAP_ROAD_NETWORK_TYPES_H
#define PHARAOH_MAP_ROAD_NETWORK_TYPES_H

#include <stdint.h>

enum { PHARAOH_ROAD_NETWORK_RANKS = 10 };
typedef struct PharaohRoadNetworkRank {
    uint32_t id, size;
} PharaohRoadNetworkRank;

#endif
