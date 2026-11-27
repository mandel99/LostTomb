#ifndef PHARAOH_BUILDING_VENUE_GEOMETRY_H
#define PHARAOH_BUILDING_VENUE_GEOMETRY_H

#include "pharaoh/building/venue_images.h"
#include <stdbool.h>
#include <stdint.h>

enum { PHARAOH_CONSTRUCTION_VENUE_GRID_CELLS = 25 };

bool pharaoh_construction_venue_is_crossing(const uint32_t *terrain,
    int32_t map_grid_start, int32_t x, int32_t y,
    int32_t origin_x, int32_t origin_y, int32_t size);

bool pharaoh_construction_venue_free_spots(
    const uint32_t occupancy[PHARAOH_CONSTRUCTION_VENUE_GRID_CELLS],
    int32_t extent, int32_t rows, int32_t columns,
    uint32_t *count, int32_t *locations);
void pharaoh_construction_venue_fill_road_grid(
    uint32_t occupancy[PHARAOH_CONSTRUCTION_VENUE_GRID_CELLS],
    const uint32_t *terrain, int32_t map_grid_start,
    int32_t x, int32_t y, int32_t building_type);

#endif
