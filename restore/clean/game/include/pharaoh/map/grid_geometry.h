#ifndef PHARAOH_MAP_GRID_GEOMETRY_H
#define PHARAOH_MAP_GRID_GEOMETRY_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

enum { PHARAOH_MAP_GRID_WIDTH = 228 };

typedef struct PharaohCityMapBounds {
    int32_t minimum_x, minimum_y;
    int32_t maximum_x, maximum_y;
    int32_t center_x, center_y;
} PharaohCityMapBounds;

/* The six derived bounds share storage with city-view queries. */
typedef struct PharaohMapGridGeometry {
    int32_t width, height;
    union {
        PharaohCityMapBounds bounds;
        struct {
            int32_t diamond_x_min, diamond_y_min;
            int32_t diamond_x_max, diamond_y_max;
            int32_t diamond_x_center, diamond_y_center;
        };
    };
} PharaohMapGridGeometry;

_Static_assert(sizeof(PharaohMapGridGeometry) == 32 &&
    offsetof(PharaohMapGridGeometry, bounds) == 8 &&
    offsetof(PharaohMapGridGeometry, diamond_y_center) == 28,
    "shared logical map geometry layout");

uint8_t pharaoh_map_grid_is_inside(const PharaohMapGridGeometry *geometry,int32_t x,int32_t y);
bool pharaoh_map_grid_size_reaches_outside(const PharaohMapGridGeometry *geometry,int32_t x,int32_t y,int32_t size);

#endif
