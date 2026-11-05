#include "approach_search_candidates.h"

extern uint32_t restore_route_approach_004f0190(uint32_t x, uint32_t y, int32_t actor);
extern uint32_t restore_route_approach_004f01c0(uint32_t x, uint32_t y, int32_t actor);
extern int32_t FUN_00420350(uint32_t x, uint32_t y, int32_t actor);
extern int32_t FUN_004e5c00(uint32_t x, uint32_t y, int32_t actor);
extern uint32_t restore_route_approach_00542a80(uint32_t x, uint32_t y, int32_t actor);
extern uint32_t restore_route_approach_004e0c00(uint32_t x, uint32_t y, int32_t actor);
extern uint32_t restore_route_member_approach(uint32_t entry, uint32_t receiver,
    uint32_t x, uint32_t y, int32_t actor);

/* 4BC1E0 preserves the complete EAX from the first nonzero child, or from
   the final child. Native member children consume ECX and RET12. */
__declspec(noinline) uint32_t restore_actor_approach_fallback_chain_result(
    uint32_t x, uint32_t y, int32_t actor)
{
    uint32_t result = restore_route_approach_004f0190(x, y, actor);
    if (result != 0) return result;
    result = (uint32_t)FUN_00420350(x, y, actor);
    if (result != 0) return result;
    result = (uint32_t)FUN_004e5c00(x, y, actor);
    if (result != 0) return result;
    result = restore_route_approach_00542a80(x, y, actor);
    if (result != 0) return result;
    result = restore_route_member_approach(0x40f070u, 0x6fd5d8u, x, y, actor);
    if (result != 0) return result;
    result = restore_route_member_approach(0x401f30u, 0x6de650u, x, y, actor);
    if (result != 0) return result;
    result = restore_route_member_approach(0x4e6e10u, 0x6de548u, x, y, actor);
    if (result != 0) return result;
    result = restore_route_member_approach(0x545be0u, 0x6f9320u, x, y, actor);
    if (result != 0) return result;
    result = restore_route_member_approach(0x5590c0u, 0x6f5648u, x, y, actor);
    if (result != 0) return result;
    result = restore_route_member_approach(0x556430u, 0x6de9e0u, x, y, actor);
    if (result != 0) return result;
    result = restore_route_member_approach(0x54fb40u, 0x6e9188u, x, y, actor);
    if (result != 0) return result;
    result = restore_route_member_approach(0x4178e0u, 0x6fd368u, x, y, actor);
    if (result != 0) return result;
    return restore_route_approach_004e0c00(x, y, actor);
}

/* 4BC480 does not write EAX after the cdecl child returns. Keep the physical
   full-result contract in this named API without creating a canonical ABI. */
__declspec(noinline) uint32_t restore_actor_approach_common_result(
    uint32_t x, uint32_t y, int32_t actor)
{
    return restore_route_approach_004f01c0(x, y, actor);
}
