#include "pharaoh/map/grid_geometry.h"

uint8_t pharaoh_map_grid_is_inside(
    const PharaohMapGridGeometry *geometry, int32_t x, int32_t y)
{
    if (x > geometry->diamond_x_center) {
        if (y <= geometry->diamond_y_center) {
            return x - y <= geometry->diamond_x_max -
                geometry->diamond_y_center;
        }
        return x + y <= geometry->diamond_x_max + 1 +
            geometry->diamond_y_center;
    }
    if (y <= geometry->diamond_y_center) {
        return geometry->diamond_x_min + geometry->diamond_y_center <=
            x + y;
    }
    return geometry->diamond_x_center - geometry->diamond_y_max <= x - y;
}

bool pharaoh_map_grid_size_reaches_outside(
    const PharaohMapGridGeometry *geometry,
    int32_t x, int32_t y, int32_t size)
{
    if (pharaoh_map_grid_is_inside(geometry, x, y) == 0) return false;
    if (x <= geometry->diamond_x_center) {
        if (y <= geometry->diamond_y_center) {
            return x - size + y <
                geometry->diamond_x_min + geometry->diamond_y_center;
        }
        return x - y - size <
            geometry->diamond_x_center - geometry->diamond_y_max;
    }
    if (y <= geometry->diamond_y_center) {
        return geometry->diamond_x_max - geometry->diamond_y_center <
            x - y + size;
    }
    return geometry->diamond_x_max + 1 + geometry->diamond_y_center <
        x + y + size;
}
