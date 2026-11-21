#ifndef PHARAOH_BUILDING_OCCUPANCY_QUERY_H
#define PHARAOH_BUILDING_OCCUPANCY_QUERY_H

#include <stdint.h>

typedef struct PharaohOccupancyOffset {
    uint32_t offset;
    uint32_t unused;
} PharaohOccupancyOffset;

typedef struct PharaohOccupancyBuilding {
    int16_t x;
    int16_t y;
} PharaohOccupancyBuilding;

typedef struct PharaohOccupancyState {
    uint32_t grid_origin;
    uint32_t last_origin;
    const PharaohOccupancyOffset *offsets;
    const uint32_t *terrain;
    const uint8_t *const *objects;
    const PharaohOccupancyBuilding *buildings;
} PharaohOccupancyState;

uint32_t pharaoh_region_has_blocking_tile(PharaohOccupancyState *state,
    int32_t x, int32_t y, int32_t width, int32_t height, uint32_t mask);
uint32_t pharaoh_region_has_active_occupant(PharaohOccupancyState *state,
    int32_t x, int32_t y, int32_t width, int32_t height);
uint32_t pharaoh_building_has_active_occupant(PharaohOccupancyState *state,
    int32_t building);

#endif
