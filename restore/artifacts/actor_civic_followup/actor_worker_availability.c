#include "worker_availability_candidates.h"

#define WKA_U(address) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(address))
#define WKA_W(address) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(address))
#define WKA_B(address) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(address))

typedef char worker_availability_requires_pointer32[sizeof(void *) == 4 ? 1 : -1];

extern uint32_t restore_acceptance_member_check_raw(uint32_t entry,
    uint32_t receiver, int32_t building);
extern int32_t FUN_00420c40(int32_t building, int32_t actor);

/* 401D40: the logical contract is AL; the existing RAW owner retains the
   physical full EAX, flags, register and caller-stack domain. */
__declspec(noinline) uint8_t restore_actor_kind230_accept(uint32_t receiver,
    int32_t building, int32_t actor, uint32_t accept, uint32_t mode)
{
    uint32_t building_offset = (uint32_t)building * 264u;
    uint32_t root = (uint32_t)building -
        (uint32_t)(int32_t)(int16_t)WKA_W(0x7999eau + building_offset);
    uint32_t root_offset = root * 264u;
    uint32_t root_link = (uint32_t)(int32_t)(int16_t)
        WKA_W(0x7999eau + root_offset);
    int32_t state = (int32_t)WKA_U(receiver + root_link * 52u + 88u);
    uint32_t vtable, target, result, actor_offset, tile;
    uint16_t route_x, route_y;
    uint8_t kind;

    if (state < 0 || state > 11) return 0;
    vtable = WKA_U(receiver);
    target = WKA_U(vtable + 36u);
    result = restore_acceptance_member_check_raw(target, receiver, building);
    if ((result & 255u) != 0) return 0;
    if (FUN_00420c40(building, actor) == 0) return 0;
    if ((accept & 255u) == 0) return 1;

    actor_offset = (uint32_t)actor * 388u;
    kind = WKA_B(0xbcd602u + actor_offset);
    if (kind == 81u) {
        WKA_U(0x799a34u + root_offset) = (uint32_t)actor;
        route_x = WKA_W(0x7999feu + root_offset);
        route_y = WKA_W(0x799a00u + root_offset);
        WKA_B(0xbcd630u + actor_offset) = 9u;
        WKA_W(0xbcd656u + actor_offset) = (uint16_t)root;
        WKA_W(0xbcd61cu + actor_offset) = route_x;
        tile = WKA_U(0x7999e4u + root_offset);
        WKA_W(0xbcd61eu + actor_offset) = route_y;
        WKA_U(0xbcd620u + actor_offset) = tile;
        WKA_W(0xbcd63cu + actor_offset) = 0;
        WKA_W(0xbcd62eu + actor_offset) = 0;
        return 1;
    }
    if (kind == 79u) {
        if ((mode & 255u) != 0) {
            WKA_B(0xbcd630u + actor_offset) = 10u;
        } else {
            route_x = WKA_W(0x7999feu + root_offset);
            route_y = WKA_W(0x799a00u + root_offset);
            WKA_B(0xbcd630u + actor_offset) = 9u;
            WKA_W(0xbcd61cu + actor_offset) = route_x;
            tile = WKA_U(0x7999e4u + root_offset);
            WKA_W(0xbcd61eu + actor_offset) = route_y;
            WKA_U(0xbcd620u + actor_offset) = tile;
            WKA_W(0xbcd63cu + actor_offset) = 0;
            WKA_W(0xbcd62eu + actor_offset) = 0;
        }
        WKA_W(0xbcd656u + actor_offset) = (uint16_t)root;
        WKA_U(0x799a34u + root_offset) = (uint32_t)actor;
        WKA_W(0xbcd62eu + actor_offset) = 0;
    }
    return 1;
}
