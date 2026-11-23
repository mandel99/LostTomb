#include "pharaoh/simulation/flood_build.h"
#include "pharaoh/map/tile_adjacency.h"

#include <string.h>

static int32_t add(int32_t a, int32_t b)
{
    return (int32_t)((uint32_t)a + (uint32_t)b);
}

void pharaoh_floods_build(PharaohFloodBuildContext *c,
    int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
    static const int32_t offsets[8] = {-228,-227,1,229,228,227,-1,-229};
    uint8_t visited[51984];
    const PharaohFloodBuildOperations *const ops = c->operations;
    int32_t left, right, top, bottom, pass = 30;
    uint8_t changed;

    ops->reset(ops->user_data);
    memset(visited, 0, sizeof(visited));
    left = add(x0 <= x1 ? x0 : x1, -1);
    right = add(x0 <= x1 ? x1 : x0, 1);
    top = add(y0 <= y1 ? y0 : y1, -1);
    bottom = add(y0 <= y1 ? y1 : y0, 1);
    if (left < 0) left = 0;
    if (right >= c->map->width) right = add(c->map->width, -1);
    if (top < 0) top = 0;
    if (bottom >= c->map->height) bottom = add(c->map->height, -1);

    do {
        /* Native snapshots the origin once per pass. Child calls may mutate
         * it; do not turn this into a fresh grid lookup for each tile. */
        int32_t tile = (int32_t)((uint32_t)c->map->origin +
            (uint32_t)top * 228u + (uint32_t)left);
        changed = 0;
        for (int32_t y = top; y <= bottom; y = add(y, 1)) {
            for (int32_t x = left; x <= right; x = add(x, 1), tile = add(tile, 1)) {
                if (visited[tile] != 0) continue;
                const uint8_t water = (uint8_t)((c->map->terrain[tile] >> 16) & 1u);
                const int32_t neighbors = pharaoh_map_terrain_count_neighbours_with(
                    c->map->terrain, tile, UINT32_C(0x10000));
                if (water == 0 && neighbors == 0) continue;
                const int32_t adjacent = pharaoh_map_terrain_count_neighbours_matching(
                    c->map->terrain, tile, 4, UINT32_C(0x90000));
                if (adjacent + neighbors == 8 &&
                    (c->map->terrain[tile] & UINT32_C(0x10004)) == 4) continue;
                int32_t group = 30;
                if (adjacent == 0) {
                    group = 0;
                    for (int32_t k = 0; k < 8; ++k) {
                        const int32_t neighbor = add(tile, offsets[k]);
                        if ((c->map->terrain[neighbor] & UINT32_C(0x10000)) == 0)
                            continue;
                        const int32_t label = visited[neighbor];
                        if (label > group && (label == 1 || label != pass)) {
                            group = label - 1;
                            if (group < 1) group = 1;
                        }
                    }
                    if (group == 0) continue;
                }
                c->map->terrain[tile] =
                    (c->map->terrain[tile] & UINT32_C(0xfdfbf7ff)) | UINT32_C(0x80);
                pharaoh_map_refresh_surrounding_empty(c->surface, x, y, 1);
                visited[tile] = (uint8_t)group;
                c->tiles[tile] = ops->insert(ops->user_data, &c->lists[group - 1],
                    tile, x, y, group, water);
                changed = 1;
                *c->has_floodplain = 1;
            }
            tile = add(tile, (int32_t)(228u - ((uint32_t)right - (uint32_t)left + 1u)));
        }
        pass = add(pass, -1);
    } while (changed != 0);
    *c->progress = 0;
    for (int32_t i = 29; i >= 0; --i) {
        if (c->lists[i].count == 0) break;
        *c->progress = add(*c->progress, 1);
    }
    pharaoh_map_refresh_all_tiles(c->surface);
}
