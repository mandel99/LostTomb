#include "transport_path_internal.h"

/* Width/stride repair of the complete src/unit_02313.c owner (0051a220).
   Neighbor order is north, east, south, west; tail stays cached on visited
   and rejected neighbors. This owner has no occupancy callback. */
uint32_t restore_transport_find_secondary_road_target(int32_t x, int32_t y,
                                                       int32_t targets, int32_t count)
{
    static const int32_t delta[4] = {-228, 1, 228, -1};
    uint32_t generation, start, current, next, distance;
    uint32_t head = 0, tail = 1, direction, index, pointer, previous;
    int32_t terrain;
    FUN_0040f8f0((uint32_t *)(uintptr_t)0xeed760u, 0x32c40u, 0);
    generation = TP_U(0xeadfd4u);
    start = TP_U(0xbca2d8u) + (uint32_t)y * 228u + (uint32_t)x;
    TP_U(0xeae004u) = start;
    TP_U(0xeadfd4u) = generation + 1u;
    TP_U(0xeed760u + start * 4u) = 1;
    TP_U(0xf203a0u) = TP_U(0xeae004u);
    TP_U(0xeae00cu) = 0;
    TP_U(0xeae008u) = 1;
    do {
        head = TP_U(0xeae00cu);
        current = TP_U(0xf203a0u + head * 4u);
        /* The list is read backwards again for every dequeued cell. It may
           alias the live queue or distance array. Signed-negative matches
           return zero rather than their wrapped one-based index. */
        if (count != 0) {
            index = (uint32_t)count - 1u;
            pointer = (uint32_t)targets + index * 4u;
            do {
                if (TP_U(pointer) == current)
                    return TP_I(index) < 0 ? 0u : index + 1u;
                pointer -= 4u;
                previous = index;
                --index;
            } while (previous != 0);
        }
        distance = TP_U(0xeed760u + current * 4u) + 1u;
        for (direction = 0; direction < 4u; ++direction) {
            next = current + (uint32_t)delta[direction];
            if (TP_U(0xeed760u + next * 4u) != 0) continue;
            terrain = TP_C(0xebab20u + next);
            if (terrain < 0 || terrain > 2) continue;
            TP_U(0xeed760u + next * 4u) = distance;
            tail = TP_U(0xeae008u);
            TP_U(0xf203a0u + tail * 4u) = next;
            ++tail;
            TP_U(0xeae008u) = tail;
            if (TP_I(tail) > 51983) {
                tail = 0;
                TP_U(0xeae008u) = 0;
            }
        }
        head = TP_U(0xeae00cu) + 1u;
        TP_U(0xeae00cu) = head;
        if (TP_I(head) > 51983) {
            head = 0;
            TP_U(0xeae00cu) = 0;
        }
    } while (TP_U(0xeae00cu) != tail);
    return 0;
}
