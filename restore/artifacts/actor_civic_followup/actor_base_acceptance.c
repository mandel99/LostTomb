#include "base_acceptance_candidates.h"
#include "building_services.h"
#include "worker_availability_candidates.h"

#define BAC_U(address) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(address))
#define BAC_W(address) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(address))
#define BAC_B(address) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(address))

typedef char base_acceptance_requires_pointer32[sizeof(void *) == 4 ? 1 : -1];

extern uint32_t restore_acceptance_member_check_eax(uint32_t entry,
    uint32_t receiver, int32_t building, uint32_t incoming_eax);
extern int32_t FUN_00420f50(int32_t building, int32_t actor);
extern int32_t FUN_00421230(int32_t building, int32_t actor);
extern int32_t FUN_00420910(int32_t building, int32_t actor);

/* Shared native follower test. An unusable old follower is cleared even for
   a query with accept==0; an active reciprocal follower rejects the caller. */
static uint8_t base_worker_leader_available(uint32_t leader,
    uint32_t leader_offset, uint8_t terminal_state)
{
    uint32_t follower = (uint32_t)(int32_t)(int16_t)
        BAC_W(0xbcd660u + leader_offset);
    uint32_t follower_offset;
    uint8_t state;
    if (follower == 0u) return 1;
    follower_offset = follower * 388u;
    if (BAC_B(0xbcd606u + follower_offset) != 0u &&
        BAC_B(0xbcd602u + follower_offset) == 85u) {
        state = BAC_B(0xbcd630u + follower_offset);
        if (state == 6u || state == 7u || state == 15u || state == 16u ||
            state == terminal_state) {
            if ((uint32_t)(int32_t)(int16_t)
                BAC_W(0xbcd660u + follower_offset) == leader) return 0;
        }
    }
    BAC_W(0xbcd660u + leader_offset) = 0;
    return 1;
}

__declspec(noinline) uint8_t restore_actor_accept_base_kind227(
    uint32_t receiver, int32_t building, int32_t actor, uint32_t accept)
{
    uint32_t building_offset = (uint32_t)building * 264u;
    uint32_t link = (uint32_t)(int32_t)(int16_t)
        BAC_W(0x7999eau + building_offset);
    uint32_t root = (uint32_t)building - link;
    uint32_t actor_offset, leader, leader_offset, target, vtable, result;
    uint32_t resource, selected_offset, tile;
    uint16_t route_x, route_y;
    uint8_t kind;

    if (BAC_U(receiver + link * 52u + 88u) != 0u) return 0;
    actor_offset = (uint32_t)actor * 388u;
    if (BAC_B(0xbcd602u + actor_offset) == 85u) {
        vtable = BAC_U(receiver);
        target = BAC_U(vtable + 36u);
        result = restore_acceptance_member_check_eax(target, receiver,
            building, link * 13u);
        if ((result & 255u) == 0u) return 0;
        leader = BAC_U(0x799a34u + building_offset);
        leader_offset = leader * 388u;
        if (BAC_B(0xbcd602u + leader_offset) != 81u ||
            BAC_B(0xbcd630u + leader_offset) != 10u) return 0;
        if (base_worker_leader_available(leader, leader_offset, 34u) == 0u)
            return 0;
        if ((accept & 255u) == 0u) return 1;
        resource = BAC_U(0x799a30u + root * 264u) == 18u ? 26u : 35u;
        result = (uint32_t)FUN_0045bb50(building, (int32_t)resource, 1);
        if (result == 0u) {
            BAC_B(0xbcd606u + actor_offset) = 2u;
            return 1;
        }
        BAC_B(0xbcd672u + actor_offset) = (uint8_t)resource;
        BAC_W(0xbcd66cu + actor_offset) = 0;
        BAC_W(0xbcd656u + actor_offset) = (uint16_t)result;
        route_x = BAC_W(0xd3b4e4u);
        BAC_W(0xbcd660u + actor_offset) = (uint16_t)leader;
        BAC_B(0xbcd630u + actor_offset) = 15u;
        BAC_W(0xbcd62eu + actor_offset) = 0;
        route_y = BAC_W(0xd3b4e0u);
        BAC_W(0xbcd61cu + actor_offset) = route_x;
        BAC_W(0xbcd61eu + actor_offset) = route_y;
        BAC_W(0xbcd660u + leader_offset) = (uint16_t)actor;
        return 1;
    }
    vtable = BAC_U(receiver);
    target = BAC_U(vtable + 36u);
    result = restore_acceptance_member_check_eax(target, receiver,
        building, vtable);
    if ((result & 255u) != 0u) return 0;
    result = (uint32_t)FUN_00420f50(building, actor);
    if (result == 0u) return 0;
    if ((accept & 255u) == 0u) return 1;
    kind = BAC_B(0xbcd602u + actor_offset);
    if (kind == 79u || kind == 81u) BAC_B(0xbcd630u + actor_offset) = 9u;
    selected_offset = result * 264u;
    BAC_U(0x799a34u + selected_offset) = (uint32_t)actor;
    route_x = BAC_W(0x7999feu + selected_offset);
    route_y = BAC_W(0x799a00u + selected_offset);
    tile = BAC_U(0x7999e4u + selected_offset);
    BAC_W(0xbcd61cu + actor_offset) = route_x;
    BAC_W(0xbcd61eu + actor_offset) = route_y;
    BAC_U(0xbcd620u + actor_offset) = tile;
    BAC_W(0xbcd63cu + actor_offset) = 0;
    BAC_W(0xbcd62eu + actor_offset) = 0;
    return 1;
}

__declspec(noinline) uint8_t restore_actor_accept_base_kind228(
    uint32_t receiver, int32_t building, int32_t actor, uint32_t accept)
{
    uint32_t building_offset = (uint32_t)building * 264u;
    uint32_t link = (uint32_t)(int32_t)(int16_t)
        BAC_W(0x7999eau + building_offset);
    uint32_t actor_offset, leader, leader_offset, target, vtable, result;
    uint32_t selected_offset, tile;
    uint16_t route_x, route_y;
    uint8_t kind;

    if (BAC_U(receiver + link * 52u + 88u) != 0u) return 0;
    vtable = BAC_U(receiver);
    actor_offset = (uint32_t)actor * 388u;
    kind = BAC_B(0xbcd602u + actor_offset);
    target = BAC_U(vtable + 36u);
    result = restore_acceptance_member_check_eax(target, receiver,
        building, link * 13u);
    if (kind == 85u) {
        if ((result & 255u) == 0u) return 0;
        leader = BAC_U(0x799a34u + building_offset);
        leader_offset = leader * 388u;
        if (BAC_B(0xbcd602u + leader_offset) != 81u ||
            BAC_B(0xbcd630u + leader_offset) != 10u) return 0;
        if (base_worker_leader_available(leader, leader_offset, 35u) == 0u)
            return 0;
        if ((accept & 255u) == 0u) return 1;
        result = (uint32_t)FUN_0045bb50(building, 35, 1);
        if (result == 0u) {
            BAC_B(0xbcd606u + actor_offset) = 2u;
            return 1;
        }
        route_y = BAC_W(0xd3b4e0u);
        BAC_B(0xbcd672u + actor_offset) = 35u;
        BAC_W(0xbcd66cu + actor_offset) = 0;
        BAC_W(0xbcd656u + actor_offset) = (uint16_t)result;
        BAC_W(0xbcd660u + actor_offset) = (uint16_t)leader;
        BAC_B(0xbcd630u + actor_offset) = 15u;
        BAC_W(0xbcd62eu + actor_offset) = 0;
        route_x = BAC_W(0xd3b4e4u);
        BAC_W(0xbcd61cu + actor_offset) = route_x;
        BAC_W(0xbcd61eu + actor_offset) = route_y;
        BAC_W(0xbcd660u + leader_offset) = (uint16_t)actor;
        return 1;
    }
    if ((result & 255u) != 0u) return 0;
    result = (uint32_t)FUN_00421230(building, actor);
    if (result == 0u) return 0;
    if ((accept & 255u) == 0u) return 1;
    kind = BAC_B(0xbcd602u + actor_offset);
    if (kind == 79u || kind == 81u) BAC_B(0xbcd630u + actor_offset) = 9u;
    selected_offset = result * 264u;
    BAC_U(0x799a34u + selected_offset) = (uint32_t)actor;
    route_x = BAC_W(0x7999feu + selected_offset);
    BAC_W(0xbcd61cu + actor_offset) = route_x;
    route_y = BAC_W(0x799a00u + selected_offset);
    tile = BAC_U(0x7999e4u + selected_offset);
    BAC_W(0xbcd61eu + actor_offset) = route_y;
    BAC_U(0xbcd620u + actor_offset) = tile;
    BAC_W(0xbcd63cu + actor_offset) = 0;
    BAC_W(0xbcd62eu + actor_offset) = 0;
    return 1;
}

__declspec(noinline) uint8_t restore_actor_accept_base_kind225(
    uint32_t receiver, int32_t building, int32_t actor, uint32_t accept)
{
    uint32_t building_offset = (uint32_t)building * 264u;
    uint32_t root = (uint32_t)building, root_offset, link, actor_offset;
    uint32_t leader, leader_offset, vtable, target, result, resource, tile;
    int32_t state, phase;
    uint16_t route_x, route_y;
    uint8_t kind;

    if ((int32_t)BAC_U(0x799a30u + building_offset) >= 3)
        root -= (uint32_t)(int32_t)(int16_t)BAC_W(0x7999eau + building_offset);
    root_offset = root * 264u;
    link = (uint32_t)(int32_t)(int16_t)BAC_W(0x7999eau + root_offset);
    state = (int32_t)BAC_U(receiver + link * 52u + 88u);
    if (state < 0 || state > 24) return 0;
    actor_offset = (uint32_t)actor * 388u;
    if (BAC_B(0xbcd602u + actor_offset) == 85u) {
        phase = (int32_t)BAC_U(0x799a30u + root_offset);
        if (phase < 2) {
            if (BAC_U(0x799a34u + root_offset) != 0u) return 0;
            if ((accept & 255u) != 0u) {
                BAC_U(0x799a34u + root_offset) = (uint32_t)actor;
                route_x = BAC_W(0x7999feu + building_offset);
                BAC_B(0xbcd630u + actor_offset) = 32u;
                BAC_W(0xbcd656u + actor_offset) = (uint16_t)root;
                BAC_W(0xbcd61cu + actor_offset) = route_x;
                route_y = BAC_W(0x799a00u + building_offset);
                tile = BAC_U(0x7999e4u + building_offset);
                BAC_W(0xbcd61eu + actor_offset) = route_y;
                BAC_U(0xbcd620u + actor_offset) = tile;
                BAC_W(0xbcd62eu + actor_offset) = 0;
                BAC_W(0xbcd63cu + actor_offset) = 0;
            }
            return 1;
        }
        if (phase < 3) return 0;
        vtable = BAC_U(receiver);
        target = BAC_U(vtable + 36u);
        result = restore_acceptance_member_check_eax(target, receiver,
            building, (uint32_t)building);
        if ((result & 255u) == 0u) return 0;
        leader = BAC_U(0x799a34u + root_offset);
        leader_offset = leader * 388u;
        if (BAC_B(0xbcd602u + leader_offset) != 81u ||
            BAC_B(0xbcd630u + leader_offset) != 10u) return 0;
        if (base_worker_leader_available(leader, leader_offset, 31u) == 0u)
            return 0;
        if ((accept & 255u) == 0u) return 1;
        phase = (int32_t)BAC_U(0x799a30u + root_offset);
        resource = phase < 16 || phase >= 20 ? 35u : 29u;
        result = (uint32_t)FUN_0045bb50(building, (int32_t)resource, 1);
        if (result == 0u) {
            BAC_B(0xbcd606u + actor_offset) = 2u;
            return 1;
        }
        BAC_B(0xbcd672u + actor_offset) = (uint8_t)resource;
        BAC_W(0xbcd66cu + actor_offset) = 0;
        route_y = BAC_W(0xd3b4e0u);
        BAC_W(0xbcd656u + actor_offset) = (uint16_t)result;
        BAC_W(0xbcd660u + actor_offset) = (uint16_t)leader;
        BAC_B(0xbcd630u + actor_offset) = 15u;
        BAC_W(0xbcd62eu + actor_offset) = 0;
        route_x = BAC_W(0xd3b4e4u);
        BAC_W(0xbcd61cu + actor_offset) = route_x;
        BAC_W(0xbcd61eu + actor_offset) = route_y;
        BAC_W(0xbcd660u + leader_offset) = (uint16_t)actor;
        return 1;
    }
    vtable = BAC_U(receiver);
    target = BAC_U(vtable + 36u);
    result = restore_acceptance_member_check_eax(target, receiver,
        building, (uint32_t)building);
    if ((result & 255u) != 0u ||
        (int32_t)BAC_U(0x799a30u + root_offset) < 3) return 0;
    if (FUN_00420910(building, actor) == 0) return 0;
    if ((accept & 255u) == 0u) return 1;
    kind = BAC_B(0xbcd602u + actor_offset);
    if (kind == 79u || kind == 81u) BAC_B(0xbcd630u + actor_offset) = 9u;
    BAC_U(0x799a34u + root_offset) = (uint32_t)actor;
    route_x = BAC_W(0x7999feu + root_offset);
    route_y = BAC_W(0x799a00u + root_offset);
    tile = BAC_U(0x7999e4u + root_offset);
    BAC_W(0xbcd656u + actor_offset) = (uint16_t)(root + 15u);
    BAC_W(0xbcd61cu + actor_offset) = route_x;
    BAC_W(0xbcd61eu + actor_offset) = route_y;
    BAC_U(0xbcd620u + actor_offset) = tile;
    BAC_W(0xbcd63cu + actor_offset) = 0;
    BAC_W(0xbcd62eu + actor_offset) = 0;
    return 1;
}

/* Exact bounded body 401EB0..401EC8: three original slots, mode forced to0. */
__declspec(noinline) uint8_t restore_actor_accept_kind230_mode0(
    uint32_t receiver, int32_t building, int32_t actor, uint32_t accept)
{
    return restore_actor_kind230_accept(receiver, building, actor, accept, 0u);
}
