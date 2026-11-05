#include "acceptance_member_candidates.h"

#define ACM_U(address) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(address))
#define ACM_W(address) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(address))
#define ACM_B(address) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(address))

typedef char acceptance_member_requires_pointer32[sizeof(void *) == 4 ? 1 : -1];

/* RAW children retain ECX, every complete stack word, and full EAX. */
extern uint32_t restore_acceptance_kind230_check_raw(uint32_t receiver,
    int32_t building, int32_t actor, uint32_t accept, uint32_t mode);
extern uint32_t restore_acceptance_member_check_eax(uint32_t entry,
    uint32_t receiver, int32_t building, uint32_t incoming_eax);
extern int32_t FUN_0045bb50(int32_t building, int32_t resource, char require_stock);
extern int32_t FUN_00422a90(int32_t building, int32_t actor);

/* 401ED0: ECX is forwarded to 401D40; X/Y are genuinely unused. */
__declspec(noinline) int32_t restore_actor_find_approach_kind230_mode(
    uint32_t receiver, uint32_t x, uint32_t y, int32_t actor, uint32_t mode)
{
    uint32_t kind_address = 0x799af0u, accepted;
    int32_t selected = 0, building = 1;
    (void)x; (void)y;
    do {
        if (selected != 0) return selected;
        if (ACM_B(kind_address - 16u) == 1u && ACM_W(kind_address) == 230u) {
            accepted = restore_acceptance_kind230_check_raw(receiver,
                building, actor, 1u, mode);
            if ((accepted & 255u) != 0) selected = building;
        }
        kind_address += 264u;
        ++building;
    } while ((int32_t)kind_address < 0x89b6e8);
    return selected;
}

/* 547C10: actor*388 and building*264 wrap as native DWORD arithmetic.
   Native SETZ overwrites the low byte of the already-read actor stack slot;
   its captured kind==85 flag and the separate mode slot must not be confused.
   The RAW owner retains that physical caller-stack/register/flags domain. */
__declspec(noinline) uint32_t restore_actor_member_accept_building_core(
    uint32_t receiver, int32_t building, int32_t actor, uint32_t mode)
{
    uint32_t actor_offset = (uint32_t)actor * 388u;
    uint32_t building_offset, root, root_stock, result, target, vtable;
    uint32_t captured_kind85, special_route = 0;
    uint16_t route_x, route_y;
    uint8_t current_kind;

    captured_kind85 = ACM_B(0xbcd602u + actor_offset) == 85u;
    vtable = ACM_U(receiver);
    target = ACM_U(vtable + 36u);
    result = restore_acceptance_member_check_eax(target, receiver, building, vtable);
    if ((result & 255u) != 0) return result & 0xffffff00u;

    if (captured_kind85 != 0) {
        building_offset = (uint32_t)building * 264u;
        if (ACM_W(0x7999eau + building_offset) == 0u &&
            ACM_B(receiver + 76u) == 0u &&
            (int32_t)ACM_U(0x799a70u + building_offset) < 400) {
            special_route = 1;
        }
    }
    if (special_route != 0) {
        result = (uint32_t)FUN_0045bb50(building, 34, 1);
        if (result == 0) return 0;
        if ((mode & 255u) == 0) return (result & 0xffffff00u) | 1u;

        route_x = ACM_W(0xd3b4e4u);
        ACM_B(0xbcd672u + actor_offset) = 34u;
        ACM_W(0xbcd66cu + actor_offset) = 0;
        ACM_W(0xbcd656u + actor_offset) = (uint16_t)result;
        ACM_W(0xbcd660u + actor_offset) = (uint16_t)actor;
        ACM_W(0xbcd654u + actor_offset) = (uint16_t)building;
        ACM_B(0xbcd630u + actor_offset) = 15u;
        ACM_W(0xbcd62eu + actor_offset) = 0;
        ACM_W(0xbcd61cu + actor_offset) = route_x;
        route_y = ACM_W(0xd3b4e0u);
        ACM_W(0xbcd61eu + actor_offset) = route_y;
        result = ACM_U(0x7999e4u + result * 264u);
        ACM_U(0xbcd620u + actor_offset) = result;
        ACM_W(0xbcd63cu + actor_offset) = 0;
        ACM_B(receiver + 76u) = 1u;
        return (result & 0xffffff00u) | 1u;
    }

    building_offset = (uint32_t)building * 264u;
    root = (uint32_t)building -
        (uint32_t)(int32_t)(int16_t)ACM_W(0x7999eau + building_offset);
    root_stock = 0x799a70u + root * 264u;
    result = ACM_U(root_stock);
    if ((int32_t)result < 100) return result & 0xffffff00u;
    result = (uint32_t)FUN_00422a90(building, actor);
    if (result == 0) return 0;
    if ((mode & 255u) == 0) return (result & 0xffffff00u) | 1u;

    current_kind = ACM_B(0xbcd602u + actor_offset);
    if (current_kind == 81u || current_kind == 108u)
        ACM_B(0xbcd630u + actor_offset) = 9u;
    ACM_U(root_stock) = ACM_U(root_stock) - 100u;
    building_offset = result * 264u;
    ACM_U(0x799a34u + building_offset) = (uint32_t)actor;
    route_x = ACM_W(0x7999feu + building_offset);
    ACM_W(0xbcd61cu + actor_offset) = route_x;
    route_y = ACM_W(0x799a00u + building_offset);
    target = ACM_U(0x7999e4u + building_offset);
    ACM_W(0xbcd61eu + actor_offset) = route_y;
    ACM_U(0xbcd620u + actor_offset) = target;
    ACM_W(0xbcd63cu + actor_offset) = 0;
    ACM_W(0xbcd62eu + actor_offset) = 0;
    return (result & 0xffff0000u) | ((uint32_t)route_y & 0xff00u) | 1u;
}
