#include "pharaoh/map/road_access.h"

/* Reused complete prior game/Map/RoadAccess.c algorithms. */
enum { RANK_UNLISTED = 11, RANK_NONE = 12, RING_TILES = 24 };
static const int32_t nearby[8][3] = {
    {-228, 0, -1}, {1, 1, 0}, {228, -1, 0}, {-1, 0, 1},
    {-456, 0, -2}, {2, 2, 0}, {456, -2, 0}, {-2, 0, 2}
};

static int32_t usable(const PharaohRoadAccessContext *c,
    uint32_t tile, int32_t reject_water)
{
    const uint32_t terrain = c->terrain[tile];
    if ((terrain & 0x40u) == 0 || (reject_water && (terrain & 4u))) return 0;
    if (terrain & 8u) {
        const int32_t kind = c->buildings[c->building_ids[tile]].building_type;
        if (kind == 0x8a || kind == 0x88) return 0;
    }
    return 1;
}

static int32_t rank(const PharaohRoadAccessContext *c, uint32_t tile)
{
    for (int32_t i = 0; i < PHARAOH_ROAD_NETWORK_RANKS; ++i)
        if (c->ranks[i].id == c->regions[tile]) return i;
    return RANK_UNLISTED;
}

static uint32_t offset(const PharaohRoadAccessContext *c, int32_t x, int32_t y)
{
    return (uint32_t)c->grid_start + (uint32_t)y * 228u + (uint32_t)x;
}

/* Native00480180: first strictly better rank wins; outputs stay on failure. */
int32_t pharaoh_map_has_road_access(PharaohRoadAccessContext *c,
    int32_t x, int32_t y, int32_t size)
{
    const uint32_t origin = offset(c, x, y);
    const int32_t start = size * RING_TILES;
    int32_t best = RANK_NONE, best_index = -1;
    for (int32_t i = 0; i < RING_TILES; ++i) {
        const int32_t delta = c->ring_offsets[start + i];
        uint32_t tile;
        int32_t candidate;
        if (delta == 0) break;
        tile = origin + (uint32_t)delta;
        if (!usable(c, tile, 1)) continue;
        candidate = rank(c, tile);
        if (candidate < best) { best = candidate; best_index = i; }
    }
    if (best > RANK_UNLISTED) return 0;
    c->result_x = x + c->ring_xy[2 * (start + best_index)];
    c->result_y = y + c->ring_xy[2 * (start + best_index) + 1];
    return 1;
}

/* Native004802C0 deliberately permits water+road, unlike00480180.
 * Possible native bug: stack delta entries2/3 and6/7 disagree with their
 * coordinate pairs (0048031D/25/35,004802DB). Keep both literal tables.
 * Prior RoadAccess.c reordered the deltas to match XY; that was not faithful. */
int32_t pharaoh_map_has_road_access_nearby(PharaohRoadAccessContext *c,
    int32_t x, int32_t y)
{
    const uint32_t origin = offset(c, x, y);
    int32_t best = RANK_NONE, best_index = -1;
    for (int32_t i = 0; i < 8; ++i) {
        const uint32_t tile = origin + (uint32_t)nearby[i][0];
        int32_t candidate;
        if (!usable(c, tile, 0)) continue;
        candidate = rank(c, tile);
        if (candidate < best) { best = candidate; best_index = i; }
    }
    if (best > RANK_UNLISTED) return 0;
    c->result_x = x + nearby[best_index][1];
    c->result_y = y + nearby[best_index][2];
    return 1;
}

/* Native00480460: columns first; interior columns visit only both end rows. */
int32_t pharaoh_map_has_road_access_temple_complex(PharaohRoadAccessContext *c,
    int32_t x, int32_t y, int32_t orientation)
{
    int32_t rows, columns, start_x, start_y;
    int32_t best = RANK_NONE, best_offset = -1;
    if (orientation == 0 || orientation == 4) {
        rows = 9; columns = 15; start_x = x - 1; start_y = y - 3;
    } else {
        rows = 15; columns = 9; start_x = x - 3; start_y = y - 1;
    }
    for (int32_t column = 0; column < columns; ++column) {
        const int32_t step = (column == 0 || column == columns - 1) ? 1 : rows - 1;
        for (int32_t row = 0; row < rows; row += step) {
            const uint32_t tile = offset(c, start_x + column, start_y + row);
            int32_t candidate;
            if (!usable(c, tile, 1)) continue;
            candidate = rank(c, tile);
            if (candidate < best) { best = candidate; best_offset = (int32_t)tile; }
        }
    }
    if (best > RANK_UNLISTED) return 0;
    best_offset = (int32_t)((uint32_t)best_offset - (uint32_t)c->grid_start);
    c->result_x = best_offset % 228;
    c->result_y = best_offset / 228;
    return 1;
}
