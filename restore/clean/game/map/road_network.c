#include "pharaoh/map/road_network.h"
#include <string.h>

/* Complete Unit00940 C reuse, native004689E0. This is an iterative ring,
 * not recursion. Cardinal order is the original table005CD678. */
void pharaoh_map_color_road_region(PharaohRoadNetworkContext *c,
    uint32_t tile, uint8_t region)
{
    static const int32_t delta[4] = {-228, 1, 228, -1};
    uint32_t tail = 0, steps = 1;
    memset(c->queue, 0, sizeof(c->queue));
    c->queue_head = 0;
    for (;;) {
        uint32_t first = UINT32_MAX;
        c->shades[tile] = (uint8_t)c->shade;
        c->regions[tile] = region;
        for (uint32_t direction = 0; direction < 4; ++direction) {
            const uint32_t neighbor = tile + (uint32_t)delta[direction];
            const int32_t classification = c->citizen_routing[neighbor];
            if (classification < 0 || classification > 2 ||
                (classification == 2 &&
                    (c->terrain[neighbor] & 0x400u) == 0) ||
                c->regions[neighbor] != 0) continue;
            c->shades[neighbor] = (uint8_t)c->shade;
            c->regions[neighbor] = region;
            ++c->region_size;
            if (first == UINT32_MAX) {
                ++c->shade_steps;
                first = neighbor;
            } else {
                c->queue[tail] = neighbor;
                if (++tail >= PHARAOH_ROAD_QUEUE_SIZE) tail = 0;
            }
        }
        if (first == UINT32_MAX) {
            if (c->queue_head == tail) break;
            first = c->queue[c->queue_head];
            if (++c->queue_head >= PHARAOH_ROAD_QUEUE_SIZE)
                c->queue_head = 0;
        }
        if ((int32_t)c->shade_steps > 30) {
            ++c->shade;
            c->shade_steps = 0;
        }
        tile = first;
        if ((int32_t)first < 0) break;
        if (steps++ >= PHARAOH_ROAD_GRID_TILES) break;
    }
    c->queue_tail = tail;
}

/* Complete Unit00939 C reuse, native00468860. Equal sizes retain scan order. */
void pharaoh_city_rebuild_road_networks(PharaohRoadNetworkContext *c)
{
    PharaohRoadNetworkRank *ranks =
        c->players[*c->current_player].largest_road_networks;
    uint32_t region = 1, row = 0;
    int32_t width;
    for (uint32_t i = 0; i < PHARAOH_ROAD_NETWORK_RANKS; ++i) {
        ranks[i].id = 0;
        ranks[i].size = 0;
    }
    memset(c->regions, 0, PHARAOH_ROAD_GRID_TILES);
    memset(c->shades, 0, PHARAOH_ROAD_GRID_TILES);
    c->shade = 1;
    c->shade_steps = 0;
    c->scan_tile = (uint32_t)c->geometry->grid_start;
    if (c->geometry->height <= 0) return;
    width = c->geometry->width;
    do {
        for (int32_t column = 0; column < width; ++column) {
            if ((c->terrain[c->scan_tile] & 0x44u) == 0x40u &&
                c->regions[c->scan_tile] == 0) {
                uint32_t i;
                c->region_size = 1;
                /* Possible native bug: region256 becomes the unvisited
                 * BYTE label0, while ranks retain the full DWORD id.
                 * Preserve the native widths; intent is unproven. */
                pharaoh_map_color_road_region(c, c->scan_tile, (uint8_t)region);
                ranks = c->players[*c->current_player].largest_road_networks;
                for (i = 0; i < PHARAOH_ROAD_NETWORK_RANKS; ++i)
                    if ((int32_t)c->region_size > (int32_t)ranks[i].size) break;
                if (i < PHARAOH_ROAD_NETWORK_RANKS) {
                    for (uint32_t j = 9; j > i; --j) ranks[j] = ranks[j - 1];
                    ranks[i].id = region;
                    ranks[i].size = c->region_size;
                }
                ++region;
                width = c->geometry->width;
            }
            ++c->scan_tile;
        }
        ++row;
        c->scan_tile += (uint32_t)c->geometry->grid_border;
    } while ((int32_t)row < c->geometry->height);
}
