#include "acceptance_predicates_internal.h"
#include "monument_complex_queries.h"
#include "mausoleum.h"

/* Logical AL repair of the existing 4E05F0 source owner. */
uint8_t restore_actor_accept_kind222_work(int32_t building, int32_t actor,
                                        uint8_t commit_changes)
{
    uint32_t building_offset = ap_building_offset(building);
    uint32_t actor_offset = ap_actor_offset((uint32_t)actor);
    uint8_t type = AP_B(0xbcd602u + actor_offset);
    uint8_t laborer = (uint8_t)(type == 0x55u);
    uint8_t type79_worker = (uint8_t)(type == 0x4fu);
    uint8_t type81_worker = (uint8_t)(type == 0x51u);
    int32_t relative = AP_S(0x7999eau + building_offset);
    uint32_t root = (uint32_t)building - (uint32_t)relative;
    uint32_t flags = AP_U(0x799a3cu + root * 264u);
    uint8_t phase1 = (uint8_t)(flags & 1u);
    uint8_t phase2 = (uint8_t)((flags >> 1) & 1u);
    uint32_t descriptor, direction, selected, work, mask, done;

    if (relative == 0 && phase2 != 0) {
        uint32_t index;
        for (index = 0; index < 4u; ++index) {
            uint32_t id = index - (uint32_t)AP_S(0x7999eau + building_offset)
                          + (uint32_t)building;
            if ((int32_t)AP_U(0x799a30u + id * 264u) < 0xffff) {
                AP_U(0x799a30u + id * 264u) = 0xffff;
                FUN_004dfbb0((int32_t)id);
            }
        }
    }
    if (type79_worker != 0 && ((flags & 0xf0u) == 0xf0u || phase1 == 0))
        return 0;

    relative = AP_S(0x7999eau + building_offset);
    descriptor = AP_U(0x5e2ae4u + (uint32_t)relative * 52u);
    switch (descriptor & 0xffu) {
    case 0: case 1: case 2: case 3: case 11: case 13:
        return 0;
    default:
        break;
    }
    direction = descriptor & 0xf00u;
    if (direction != 0 && (int32_t)AP_U(0x799a30u + building_offset) > 0 &&
        phase2 == 0) {
        switch (direction) {
        case 0x100: mask = 0x10u; selected = 0; break;
        case 0x200: mask = 0x20u; selected = 1; break;
        case 0x400: mask = 0x40u; selected = 2; break;
        case 0x800: mask = 0x80u; selected = 3; break;
        default: return 0;
        }
        if ((flags & mask) != 0) {
            if (type79_worker != 0) return 0;
        } else if (type81_worker != 0 && phase1 != 0) {
            return 0;
        }
        selected += (uint32_t)building -
                    (uint32_t)AP_S(0x7999eau + building_offset);
        if (laborer != 0) {
            uint32_t worker, worker_offset;
            if (FUN_004e0530((uint32_t)building) == 0) return 0;
            worker = AP_U(0x799a34u + building_offset);
            worker_offset = ap_actor_offset(worker);
            if (AP_B(0xbcd602u + worker_offset) != 0x51u ||
                AP_B(0xbcd630u + worker_offset) != 10u) return 0;
            if (ap_linked_laborer_blocks(worker, 0x1a) == 0) {
                if (commit_changes != 0)
                    ap_commit_linked_delivery(building, actor, worker,
                                              AP_SOURCE_X_THEN_LATE_Y);
                return 1;
            }
            /* The native live-link path falls through to its final laborer
               check, including a second worker-query call when reached. */
        } else if (type81_worker != 0) {
            work = AP_U(0x799a30u + selected * 264u);
            switch (descriptor & 0xf000u) {
            case 0x1000: mask = 3u; done = 0x300u; break;
            case 0x2000: mask = 0xcu; done = 0xc00u; break;
            case 0x4000: mask = 0x30u; done = 0x3000u; break;
            case 0x8000: mask = 0xc0u; done = 0xc000u; break;
            default: return 0;
            }
            if ((work & mask) == mask &&
                ((work & done) == done || phase1 == 0)) return 0;
            if (FUN_004e0530((uint32_t)building) != 0) return 0;
            if (commit_changes != 0) {
                AP_B(0xbcd630u + actor_offset) = 9;
                ap_claim_destination(building_offset, actor_offset, actor);
            }
            return 1;
        } else {
            if (type79_worker == 0 || FUN_004e0530((uint32_t)building) != 0)
                return 0;
            if (commit_changes != 0) {
                AP_B(0xbcd630u + actor_offset) = 9;
                ap_claim_destination(building_offset, actor_offset, actor);
            }
            return 1;
        }
    }
    if (laborer == 0) return 0;
    work = AP_U(0x799a30u + building_offset);
    if (work == 2u || (work == 1u &&
        (((flags >> 2) & 1u) == 0 || phase2 == 0))) return 0;
    if (FUN_004e0530((uint32_t)building) != 0) return 0;
    if (commit_changes != 0) {
        AP_B(0xbcd630u + actor_offset) = 27;
        ap_claim_destination(building_offset, actor_offset, actor);
    }
    return 1;
}

/* Keep the existing cdecl full-result surface on the physical RAW path. */
extern uint32_t restore_actor_approach_kind222_accept_raw(int32_t,int32_t,char);
uint32_t FUN_004e05f0(int32_t building,int32_t actor,char mode)
{return restore_actor_approach_kind222_accept_raw(building,actor,mode);}
