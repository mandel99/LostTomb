#include "pharaoh/map/surface_queries.h"

uint8_t pharaoh_map_sweep_has_passed(
    int32_t countdown, int32_t sweep_start, int32_t column)
{
    return countdown > 0 &&
        228 - column < (sweep_start - countdown) / 2;
}

int32_t pharaoh_map_road_canal_crossing_image(
    uint32_t unused_grid_offset, int32_t image_offset)
{
    (void)unused_grid_offset;
    switch (image_offset) {
    case 0x34: return 0x56;
    case 0x30: return 0x54;
    case 0x36: return 0x57;
    case 0x32: return 0x55;
    default: return image_offset;
    }
}
