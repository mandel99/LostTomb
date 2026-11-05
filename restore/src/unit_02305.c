#include "transport_path_internal.h"

/* Width/stride repair of the complete src/unit_02305.c owner (005190a0).
   Targets are marked in list order with -1, -2, ...; duplicates retain the
   later index and the starting cell's marker is overwritten by distance 1.
   North/east/south hits store distance before returning; west hits do not. */
uint32_t restore_transport_find_marked_free_land_target(int32_t x, int32_t y,
                                                      int32_t *targets, int32_t count)
{
    static const int32_t delta[4] = {-228, 1, 228, -1};
    uint32_t generation, start, current, next, distance;
    uint32_t pointer = (uint32_t)(uintptr_t)targets, remaining = (uint32_t)count;
    uint32_t marker = 0xffffffffu, found = 0xffffffffu;
    uint32_t head = 0, tail = 1, direction;
    int32_t terrain;
    FUN_0040f8f0((uint32_t *)(uintptr_t)0xeed760u, 0x32c40u, 0);
    if (count <= 0) return 0;
    do {
        next = TP_U(pointer);
        pointer += 4u;
        TP_U(0xeed760u + next * 4u) = marker;
        --marker;
    } while (--remaining != 0);
    generation = TP_U(0xeadfd4u);
    start = TP_U(0xbca2d8u) + (uint32_t)y * 228u + (uint32_t)x;
    TP_U(0xeae004u) = start;
    TP_U(0xeadfd4u) = generation + 1u;
    TP_U(0xeed760u + start * 4u) = 1;
    TP_U(0xf203a0u) = TP_U(0xeae004u);
    TP_U(0xeae00cu) = 0;
    TP_U(0xeae008u) = 1;
    do {
        current = TP_U(0xf203a0u + head * 4u);
        distance = TP_U(0xeed760u + current * 4u) + 1u;
        for (direction = 0; direction < 4u; ++direction) {
            next = current + (uint32_t)delta[direction];
            if (TP_I(TP_U(0xeed760u + next * 4u)) > 0) continue;
            if (FUN_0051aff0(TP_I(next)) == 0) {
                tail = TP_U(0xeae008u);
                continue;
            }
            terrain = TP_C(0xee0c50u + next);
            if (terrain < 0 && terrain != -3 &&
                (terrain != -1 || (TP_U(0xaf598cu + next * 4u) & 0x1000u) == 0)) {
                tail = TP_U(0xeae008u);
                continue;
            }
            if (terrain > 4 || terrain == 3) {
                tail = TP_U(0xeae008u);
                continue;
            }
            /* Re-read after the child. It can change the marker or globals;
               the initial visited test is not repeated. */
            found = 0xffffffffu - TP_U(0xeed760u + next * 4u);
            if (direction != 3u) TP_U(0xeed760u + next * 4u) = distance;
            if (TP_I(found) >= 0) return found + 1u;
            if (direction == 3u) TP_U(0xeed760u + next * 4u) = distance;
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
    } while (head != tail);
    return TP_I(found) >= 0 ? found + 1u : 0u;
}
