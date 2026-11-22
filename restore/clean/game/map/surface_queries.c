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


static int32_t square_matches(
    const PharaohMapTerrainView *map, int32_t x, int32_t y,
    int32_t size, uint32_t mask, int32_t require_missing)
{
    const int32_t min_x = (int32_t)((uint32_t)x + 1u - (uint32_t)size);
    const int32_t max_x = (int32_t)((uint32_t)x + (uint32_t)size - 1u);
    const int32_t min_y = (int32_t)((uint32_t)y + 1u - (uint32_t)size);
    const int32_t max_y = (int32_t)((uint32_t)y + (uint32_t)size - 1u);
    uint32_t tile = (uint32_t)map->origin +
        (uint32_t)min_y * PHARAOH_MAP_GRID_WIDTH + (uint32_t)min_x;
    const uint32_t row_step = (uint32_t)min_x - (uint32_t)max_x +
        PHARAOH_MAP_GRID_WIDTH - 1u;
    uint32_t row = (uint32_t)min_y;

    if (min_y > max_y) return 0;
    do {
        uint32_t column = (uint32_t)min_x;
        if (min_x <= max_x) {
            do {
                if ((int32_t)column >= 0 && (int32_t)row >= 0 &&
                    (int32_t)column < map->width &&
                    (int32_t)row < map->height) {
                    const int32_t missing =
                        (map->terrain[tile] & mask) == 0u;
                    if (missing == require_missing) return 1;
                }
                ++column;
                ++tile;
            } while ((int32_t)column <= max_x);
        }
        tile += row_step;
        ++row;
    } while ((int32_t)row <= max_y);
    return 0;
}

int32_t pharaoh_map_terrain_any_tile_in_square_with(
    const PharaohMapTerrainView *map, int32_t x, int32_t y,
    int32_t size, uint32_t mask)
{
    return square_matches(map, x, y, size, mask, 0);
}

int32_t pharaoh_map_terrain_any_tile_in_square_without(
    const PharaohMapTerrainView *map, int32_t x, int32_t y,
    int32_t size, uint32_t mask)
{
    return square_matches(map, x, y, size, mask, 1);
}

int32_t pharaoh_map_terrain_row_has_two(
    const PharaohMapTerrainView *map, int32_t x, int32_t y, uint32_t mask)
{
    uint32_t count = 0;
    uint32_t column = (uint32_t)x - 1u;
    const uint32_t end = (uint32_t)x + 2u;
    if ((int32_t)column >= (int32_t)end) return 0;
    do {
        if ((int32_t)column >= 0 && (int32_t)column < map->width) {
            const uint32_t tile = (uint32_t)map->origin +
                (uint32_t)y * PHARAOH_MAP_GRID_WIDTH + column;
            if ((map->terrain[tile] & mask) != 0u) ++count;
        }
        ++column;
    } while ((int32_t)column < (int32_t)end);
    return count > 1u;
}

int32_t pharaoh_map_terrain_column_has_two(
    const PharaohMapTerrainView *map, int32_t x, int32_t y, uint32_t mask)
{
    uint32_t count = 0;
    uint32_t row = (uint32_t)y - 1u;
    const uint32_t end = (uint32_t)y + 2u;
    uint32_t tile = (uint32_t)map->origin +
        row * PHARAOH_MAP_GRID_WIDTH + (uint32_t)x;
    if ((int32_t)row >= (int32_t)end) return 0;
    do {
        if ((int32_t)row >= 0 && (int32_t)row < map->height &&
            (map->terrain[tile] & mask) != 0u) {
            ++count;
        }
        ++row;
        tile += PHARAOH_MAP_GRID_WIDTH;
    } while ((int32_t)row < (int32_t)end);
    return count > 1u;
}

uint8_t pharaoh_map_sweep_matches_tile(
    int32_t countdown, int32_t sweep_start, int32_t tile)
{
    return countdown > 0 && (sweep_start - countdown) / 2 == 228 - tile;
}

/* Native00482810. Failure retains the last scan coordinates; no invented
 * failure fallback is added to its native entry/exit-marker caller. */
int32_t pharaoh_map_find_free_terrain(const PharaohMapTerrainView *map,int32_t x,int32_t y,
    int32_t size,int32_t radius,int32_t excluded,PharaohMapAreaCursor *cursor)
{
    int32_t left,top,right,bottom;
    area_bounds(map,x,y,size,radius,&left,&top,&right,&bottom);
    cursor->offset=map->origin+top*228+left;
    for(cursor->y=top;cursor->y<=bottom;++cursor->y){
        for(cursor->x=left;cursor->x<=right;++cursor->x){
            if(cursor->offset!=excluded && !map->terrain[cursor->offset])return 1;
            ++cursor->offset;
        }
        cursor->offset+=228-(right-left+1);
    }
    return 0;
}
