#include "route_helper_internal.h"

/* Width/stride repair of the complete original 519A30 owner. Its native
   void facade and RAW model retain incidental physical EAX/flags. */
void restore_transport_build_road_distance_map(int32_t x, int32_t y)
{
    static const int32_t delta[4] = {-228, 1, 228, -1};
    uint32_t generation, start, current, next, distance, direction;
    uint32_t head = 0, tail = 1;
    int32_t terrain;

    FUN_0040f8f0((uint32_t *)(uintptr_t)0xeed760u, 0x32c40u, 0);
    generation = RH_U(0xeadfd4u);
    start = RH_U(0xbca2d8u) + (uint32_t)y * 228u + (uint32_t)x;
    RH_U(0xeae004u) = start;
    RH_U(0xeadfd4u) = generation + 1u;
    RH_U(0xeed760u + start * 4u) = 1;
    RH_U(0xf203a0u) = start;
    RH_U(0xeae00cu) = 0;
    RH_U(0xeae008u) = 1;
    do {
        current = RH_U(0xf203a0u + head * 4u);
        distance = RH_U(0xeed760u + current * 4u) + 1u;
        for (direction = 0; direction < 4u; ++direction) {
            next = current + (uint32_t)delta[direction];
            if (RH_U(0xeed760u + next * 4u) != 0) continue;
            terrain = RH_C(0xee0c50u + next);
            if (terrain < 0 || terrain > 2) continue;
            RH_U(0xeed760u + next * 4u) = distance;
            tail = RH_U(0xeae008u);
            RH_U(0xf203a0u + tail * 4u) = next;
            ++tail;
            RH_U(0xeae008u) = tail;
            if (RH_I(tail) > 51983) {
                tail = 0;
                RH_U(0xeae008u) = 0;
            }
        }
        head = RH_U(0xeae00cu) + 1u;
        RH_U(0xeae00cu) = head;
        if (RH_I(head) > 51983) {
            head = 0;
            RH_U(0xeae00cu) = 0;
        }
    } while (head != tail);
}
