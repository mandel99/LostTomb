#include "pharaoh/monuments/progress_reports.h"

/* Complete Unit02098 C reuse, native004EEA10. AL is the logical result.
 * Recognized but unrelated live actions retain the claim, exactly as native. */
uint8_t pharaoh_pyramid_worker_claim_active(PharaohPyramidSummaryContext *c,
    int32_t building)
{
    PharaohBuildingTypeRecord *b = &c->buildings[building];
    const int32_t worker = b->pyramid_worker_id_64;
    const PharaohWalkerQueryRecord *w;
    uint32_t state;
    if (!worker) return 0;
    w = &c->walkers[worker];
    if (!w->state) goto clear;
    state = w->action_state;
    if (state == 6 || state == 7) state = w->action_state_before_6_7;
    switch (w->type) {
    case 79: return state == 9 || state == 12 || state == 10;
    case 80: return state == 10 || state == 13 || state == 15 ||
        state == 12 || state == 14 || state == 11;
    case 81:
        if (pharaoh_pyramid_available_worker_slot(c->buildings, building,
            (uint32_t)*c->style, c->descriptor_counts, c->descriptors))
            goto clear;
        if (state == 9 || state == 13 || state == 15 || state == 11 ||
            state == 14 || state == 16) return 1;
        if (state == 10) {
            if (c->tile_building_ids[w->destination_grid_offset] == building)
                return 1;
            goto clear;
        }
        return 0;
    case 85: return state == 15 || state == 16 || state == 17 ||
        state == 30 || state == 18 || state == 19 || state == 20 ||
        state == 21 || state == 29 || state == 22;
    default: return 0;
    }
clear:
    b->pyramid_worker_id_64 = 0;
    return 0;
}
