#include "pharaoh/map/surface_queries.h"
#include "pharaoh/map/grid_geometry.h"

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


static void area_bounds(const PharaohMapTerrainView *map,
    int32_t x, int32_t y, int32_t size, int32_t radius,
    int32_t *left, int32_t *top, int32_t *right, int32_t *bottom)
{
    *left = x - radius;
    *top = y - radius;
    *right = x + size - 1 + radius;
    *bottom = y + size - 1 + radius;
    if (*left < 0) *left = 0;
    if (*top < 0) *top = 0;
    if (*right >= map->width) *right = map->width - 1;
    if (*bottom >= map->height) *bottom = map->height - 1;
}

static int32_t map_area_test(const PharaohMapTerrainView *map,
    int32_t x, int32_t y, int32_t size, int32_t radius, uint32_t mask,
    int32_t require_all, PharaohMapAreaCursor *cursor)
{
    int32_t left, top, right, bottom;
    area_bounds(map, x, y, size, radius, &left, &top, &right, &bottom);
    cursor->offset = map->origin + top * PHARAOH_MAP_GRID_WIDTH + left;
    for (cursor->y = top; cursor->y <= bottom; ++cursor->y) {
        for (cursor->x = left; cursor->x <= right; ++cursor->x) {
            const int32_t present = (map->terrain[cursor->offset] & mask) != 0;
            if (present != require_all) return require_all ? 0 : 1;
            ++cursor->offset;
        }
        cursor->offset += PHARAOH_MAP_GRID_WIDTH - (right - left + 1);
    }
    return require_all ? 1 : 0;
}

bool pharaoh_map_plaza_square_fits(const uint32_t *terrain,
    const uint8_t *property_flags, const uint32_t *image_ids,
    int32_t tile)
{
    static const int32_t delta[3] = {1, 228, 229};
    uint32_t score = 0;
    int32_t index;

    for (index = 0; index < 3; ++index) {
        const int32_t neighbour = tile + delta[index];
        const uint32_t flags = terrain[neighbour];
        if ((flags & 0x40u) != 0) ++score;
        if ((property_flags[neighbour] & 0x80u) == 0) --score;
        if ((flags & 4u) != 0) --score;
        if ((flags & 8u) != 0) --score;
        if ((flags & 0x100u) != 0) --score;
        if (image_ids[neighbour] != 0) --score;
    }
    return score == 3u;
}

uint8_t pharaoh_map_last_tile_is_buildable_for_editor(
    const uint32_t *terrain, const int8_t *height_values, int32_t last_tile)
{
    if ((terrain[last_tile] & 0x10000u) != 0) return 0;
    if (height_values[last_tile] > 4) return 1;
    if (height_values[last_tile] < 1) return 0;
    return (uint8_t)((terrain[last_tile] >> 13) & 1u);
}

int32_t pharaoh_map_road_canal_floodplain_image(int32_t image_offset)
{
    return (int32_t)((uint32_t)image_offset + 0x15u);
}

int32_t pharaoh_map_terrain_exists_in_area_with(
    const PharaohMapTerrainView *map, int32_t x, int32_t y,
    int32_t size, int32_t radius, uint32_t mask,
    PharaohMapAreaCursor *cursor)
{
    return map_area_test(map, x, y, size, radius, mask, 0, cursor);
}

int32_t pharaoh_map_terrain_all_in_area_are(
    const PharaohMapTerrainView *map, int32_t x, int32_t y,
    int32_t size, int32_t radius, uint32_t mask,
    PharaohMapAreaCursor *cursor)
{
    return map_area_test(map, x, y, size, radius, mask, 1, cursor);
}
