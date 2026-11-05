#include "approach_member_candidates.h"

#define AM_U(address) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(address))
#define AM_W(address) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(address))
#define AM_B(address) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(address))

typedef char approach_member_requires_pointer32[sizeof(void *) == 4 ? 1 : -1];

/* Existing RAW child bridge: ECX receiver, three complete stack words, RET12,
   and the complete modeled EAX result. It also serves dynamic vtable targets. */
extern uint32_t restore_route_member_approach(uint32_t entry, uint32_t receiver,
    uint32_t first, uint32_t second, int32_t third);

/* Existing RAW 401ED0 child: ECX receiver, four complete stack words, RET16. */
extern uint32_t restore_actor_member_find_kind230_raw(uint32_t receiver,
    uint32_t x, uint32_t y, int32_t actor, uint32_t mode);

/* Native 4178E0/4E6E10/40F070 share this scan. X/Y are unused by all three.
   A selected entry still advances the cursor before the next loop test.
   Only AL from the dynamic acceptance child decides selection. */
static int32_t find_approach_kind(uint32_t receiver, uint16_t kind, int32_t actor)
{
    uint32_t kind_address = 0x799af0u, vtable, target, accepted;
    int32_t selected = 0, building = 1;
    do {
        if (selected != 0) return selected;
        if (AM_B(kind_address - 16u) == 1u && AM_W(kind_address) == kind) {
            vtable = AM_U(receiver);
            target = AM_U(vtable + 44u);
            accepted = restore_route_member_approach(target, receiver,
                (uint32_t)building, (uint32_t)actor, 1);
            if ((accepted & 255u) != 0) selected = building;
        }
        kind_address += 264u;
        ++building;
    } while ((int32_t)kind_address < 0x89b6e8);
    return selected;
}

__declspec(noinline) int32_t restore_actor_find_approach_kind227(
    uint32_t receiver, uint32_t x, uint32_t y, int32_t actor)
{
    (void)x; (void)y;
    return find_approach_kind(receiver, 227, actor);
}

__declspec(noinline) int32_t restore_actor_find_approach_kind228(
    uint32_t receiver, uint32_t x, uint32_t y, int32_t actor)
{
    (void)x; (void)y;
    return find_approach_kind(receiver, 228, actor);
}

__declspec(noinline) int32_t restore_actor_find_approach_kind225(
    uint32_t receiver, uint32_t x, uint32_t y, int32_t actor)
{
    (void)x; (void)y;
    return find_approach_kind(receiver, 225, actor);
}

/* 401F30 passes its incoming ECX through to 401ED0 and supplies mode zero.
   The wrapper forwards the complete EAX result. */
__declspec(noinline) uint32_t restore_actor_find_approach_kind230(
    uint32_t receiver, uint32_t x, uint32_t y, int32_t actor)
{
    return restore_actor_member_find_kind230_raw(receiver, x, y, actor, 0);
}

/* 559110 passes ECX and all three stack words unchanged to 547C10.
   Keep its complete result even though the connected scanners test only AL. */
__declspec(noinline) uint32_t restore_actor_member_accept_building(
    uint32_t receiver, int32_t building, int32_t actor, uint32_t mode)
{
    return restore_route_member_approach(0x547c10u, receiver,
        (uint32_t)building, (uint32_t)actor, (int32_t)mode);
}
