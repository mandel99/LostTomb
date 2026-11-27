#include "pharaoh/map/water_routing.h"
#include "pharaoh/core/memory_fill.h"

static void water_enqueue(const PharaohWaterRoutingContext *c, uint32_t cell)
{
    uint32_t tail = *c->queue_tail;
    c->queue[tail] = cell;
    ++tail;
    *c->queue_tail = tail;
    if ((int32_t)tail > 51983) *c->queue_tail = 0;
}

static void water_advance(const PharaohWaterRoutingContext *c)
{
    const uint32_t head = *c->queue_head + 1u;
    *c->queue_head = head;
    if ((int32_t)head > 51983) *c->queue_head = 0;
}

static void water_neighbor(const PharaohWaterRoutingContext *c,
    uint32_t next, uint32_t cost)
{
    if (c->distance[next] == 0 && c->routing_tiles[next] != 0xff &&
        c->routing_tiles[next] != 0xfd) {
        c->distance[next] = cost;
        if (c->routing_tiles[next] == 0xfe) c->distance[next] = cost + 4u;
        water_enqueue(c, next);
    }
}

/* Complete C reuse of active src/Unit02335.c and inactive mixed counterpart.
 * The native four neighbor blocks retain their north/east/south/west order. */
uint32_t pharaoh_water_routing_build_distance(
    const PharaohWaterRoutingContext *c, int32_t x, int32_t y)
{
    uint32_t iterations = 0;
    pharaoh_memory_fill_bytes(c->distance, 0x32c40, 0);
    const uint32_t start = (uint32_t)*c->origin + (uint32_t)y * 228u + (uint32_t)x;
    *c->start = start;
    if (c->routing_tiles[start] == 0xfc) return 0;
    pharaoh_memory_fill_bytes((uint32_t *)(void *)c->deferred_visits, 0xcb10, 0);
    c->distance[start] = 1;
    c->queue[0] = start;
    *c->queue_head = 0;
    *c->queue_tail = 1;
    do {
        if (++iterations > 50000u) return 1;
        const uint32_t cell = c->queue[*c->queue_head];
        if (c->routing_tiles[cell] == 0xfe) {
            const int8_t old = (int8_t)c->deferred_visits[cell];
            c->deferred_visits[cell] = (uint8_t)((uint8_t)old + 1u);
            if (old < 4) {
                water_enqueue(c, cell);
                water_advance(c);
                continue;
            }
        }
        const uint32_t cost = c->distance[cell] + 1u;
        water_neighbor(c, cell - 228u, cost);
        water_neighbor(c, cell + 1u, cost);
        water_neighbor(c, cell + 228u, cost);
        water_neighbor(c, cell - 1u, cost);
        water_advance(c);
    } while (*c->queue_head != *c->queue_tail);
    return 1;
}
