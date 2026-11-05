#include "acceptance_predicates_internal.h"
#include "monument_complex_queries.h"

/* Exact 420430 two-slot endpoint: native case0 tests its complete EAX. */
extern uint32_t restore_actor_approach_kind213_stage_worker_available_raw(
    int32_t building, int32_t actor);

/* Logical AL repair of the existing 5421D0 source owner. */
uint8_t restore_actor_accept_kind213_work(int32_t building, int32_t actor,
                                        uint8_t commit_changes)
{
    uint32_t actor_offset = ap_actor_offset((uint32_t)actor);
    uint8_t laborer = (uint8_t)(AP_B(0xbcd602u + actor_offset) == 0x55u);
    uint32_t building_offset = ap_building_offset(building);
    int32_t relative = AP_S(0x7999eau + building_offset);
    uint32_t root = (uint32_t)building - (uint32_t)relative;
    uint32_t flags = AP_U(0x799a3cu + root * 264u);
    uint32_t kind = AP_U(0x601de4u + (uint32_t)relative * 52u);
    uint8_t phase1 = (uint8_t)(flags & 1u);
    uint8_t phase2 = (uint8_t)((flags >> 1) & 1u);
    uint32_t work, worker;
    enum ap_source_coordinate_phase source_phase;

    switch (kind) {
    case 0:
        if (FUN_00542110((uint32_t)building) != 0) return 0;
        if (restore_actor_approach_kind213_stage_worker_available_raw(
                building, actor) == 0) return 0;
        if (commit_changes != 0) {
            uint8_t type = AP_B(0xbcd602u + actor_offset);
            if (type == 0x4fu) AP_B(0xbcd630u + actor_offset) = 10;
            else if (type == 0x51u) AP_B(0xbcd630u + actor_offset) = 11;
            ap_claim_destination(building_offset, actor_offset, actor);
            AP_W(0xbcd62eu + actor_offset) = 0;
        }
        return 1;

    case 1:
        if (phase1 == 0) goto initial_laborer;
        work = AP_U(0x799a30u + building_offset);
        if (work == 0) goto initial_laborer;
        if (work != 1u) return 0;
        source_phase = AP_SOURCE_Y_THEN_LATE_X;
        goto occupied_stage;

    case 2: case 3: case 4: case 5: case 6: case 7:
        if (((flags >> 2) & 1u) == 0) goto initial_laborer;
        work = AP_U(0x799a30u + building_offset);
        if (work == 0) goto initial_laborer;
        if (work != 1u) return 0;
        source_phase = AP_SOURCE_X_THEN_LATE_Y;
        goto occupied_stage;

    case 8: case 9: case 10: case 11:
        if (laborer == 0 || FUN_00542110((uint32_t)building) != 0)
            return 0;
        if (phase2 == 0) {
            if (AP_U(0x799a30u + building_offset) != 0) return 0;
        } else if (phase1 == 0 || AP_U(0x799a30u + building_offset) != 1u) {
            return 0;
        }
        if (commit_changes != 0) {
            AP_B(0xbcd630u + actor_offset) = 24;
            ap_claim_destination(building_offset, actor_offset, actor);
        }
        return 1;

    case 12:
        if (((flags >> 4) & 1u) == 0 || ((flags >> 3) & 1u) == 0)
            goto initial_laborer;
        work = AP_U(0x799a30u + building_offset);
        if (work == 0) goto initial_laborer;
        if (work != 1u && work != 2u && work != 3u) return 0;
        source_phase = AP_SOURCE_X_AND_Y_EARLY;
        goto occupied_stage;

    default:
        return 0;
    }

initial_laborer:
    if (AP_U(0x799a30u + building_offset) != 0 || laborer == 0 ||
        FUN_00542110((uint32_t)building) != 0) return 0;
    if (commit_changes != 0) {
        AP_B(0xbcd630u + actor_offset) = 24;
        ap_claim_destination(building_offset, actor_offset, actor);
    }
    return 1;

occupied_stage:
    if (laborer == 0) {
        if (FUN_00542110((uint32_t)building) != 0 ||
            AP_B(0xbcd602u + actor_offset) != 0x51u) return 0;
        if (commit_changes != 0) {
            AP_B(0xbcd630u + actor_offset) = 9;
            ap_claim_destination(building_offset, actor_offset, actor);
        }
        return 1;
    }
    if (FUN_00542110((uint32_t)building) == 0) return 0;
    worker = AP_U(0x799a34u + building_offset);
    if (ap_linked_laborer_blocks(worker, 0x17) != 0) return 0;
    if (commit_changes != 0)
        ap_commit_linked_delivery(building, actor, worker, source_phase);
    return 1;
}

/* Keep the existing cdecl full-result surface on the physical RAW path. */
extern uint32_t restore_actor_approach_kind213_accept_raw(int32_t,int32_t,char);
uint32_t FUN_005421d0(int32_t building,int32_t actor,char mode)
{return restore_actor_approach_kind213_accept_raw(building,actor,mode);}
