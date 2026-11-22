#include "pharaoh/map/access_routing.h"
#include "pharaoh/core/memory_fill.h"

static uint32_t advance(int32_t *cursor)
{
    uint32_t next = (uint32_t)*cursor + 1u;
    *cursor = (int32_t)next;
    if ((int32_t)next >= PHARAOH_ROUTING_GRID_CELLS) {
        *cursor = 0;
        next = 0;
    }
    return next;
}

static uint32_t begin(PharaohRoutingSearchState *s, int32_t x, int32_t y,
    int32_t target_x, int32_t target_y)
{
    pharaoh_memory_fill_bytes((uint32_t *)(void *)s->distance, 0x32c40, 0);
    s->source_offset = (int32_t)((uint32_t)s->grid_start +
        (uint32_t)y * PHARAOH_ROUTING_GRID_WIDTH + (uint32_t)x);
    s->total_routes_calculated =
        (int32_t)((uint32_t)s->total_routes_calculated + 1u);
    s->distance[s->source_offset] = 1;
    s->queue_items[0] = s->source_offset;
    s->queue_head = 0;
    s->queue_tail = 1;
    return (uint32_t)s->grid_start +
        (uint32_t)target_y * PHARAOH_ROUTING_GRID_WIDTH + (uint32_t)target_x;
}

static void enqueue(PharaohRoutingSearchState *s, uint32_t tile,
    uint32_t distance)
{
    s->distance[tile] = (int32_t)distance;
    s->queue_items[s->queue_tail] = (int32_t)tile;
}

/* Complete native00517A40. No road/terrain test accompanies landCitizen>=0. */
uint8_t pharaoh_map_routing_citizen_access(PharaohRoutingSearchState *s,
    int32_t x, int32_t y, int32_t target_x, int32_t target_y)
{
    static const int32_t delta[4] = {-228, 1, 228, -1};
    const uint32_t target = begin(s, x, y, target_x, target_y);
    uint32_t tail = 1;
    do {
        const uint32_t tile = (uint32_t)s->queue_items[s->queue_head];
        if (tile == target) break;
        const uint32_t distance = (uint32_t)s->distance[tile] + 1u;
        for (uint32_t i = 0; i < 4; ++i) {
            const uint32_t next = tile + (uint32_t)delta[i];
            if (s->distance[next] == 0 && s->land_citizen[next] >= 0) {
                enqueue(s, next, distance);
                tail = advance(&s->queue_tail);
            }
        }
        (void)advance(&s->queue_head);
    } while ((uint32_t)s->queue_head != tail);
    return (uint8_t)(s->distance[target] != 0);
}

/* Reused AccessRepairFlood C; complete native00517C20. The first blocked
 * wall/canal/garden is cleared and returns immediately, before head advance. */
uint32_t pharaoh_map_routing_clear_first_blocker(PharaohAccessRepairMap *m,
    int32_t x, int32_t y, int32_t target_x, int32_t target_y)
{
    static const int32_t delta[4] = {-228, 1, 228, -1};
    PharaohRoutingSearchState *const s = m->routing;
    const uint32_t target = begin(s, x, y, target_x, target_y);
    uint32_t tail = 1;
    do {
        const uint32_t tile = (uint32_t)s->queue_items[s->queue_head];
        if (tile == target) break;
        const uint32_t distance = (uint32_t)s->distance[tile] + 1u;
        for (uint32_t i = 0; i < 4; ++i) {
            const uint32_t next = tile + (uint32_t)delta[i];
            if (s->distance[next] != 0) continue;
            if (s->land_citizen[next] >= 0) {
                enqueue(s, next, distance);
                tail = advance(&s->queue_tail);
            } else if ((m->terrain[next] & 0x4120u) != 0) {
                m->edges[next] |= 0x40;
                m->terrain[next] &= 0x03052e80u;
                m->footprints[next] &= 0xf0;
                return 0;
            }
        }
        (void)advance(&s->queue_head);
    } while ((uint32_t)s->queue_head != tail);
    return (uint32_t)(s->distance[target] != 0);
}
