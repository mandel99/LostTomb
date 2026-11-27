#include "pharaoh/military/legion_requests.h"

enum { PHARAOH_TRANSPORT_SHIP_WALKER_TYPE=77 };

int32_t pharaoh_walker_find_legion_transport_ship(
    PharaohLegionRequestContext *context, int32_t formation_id)
{
    int32_t walker_id;
    *context->current_walker_id = 1;
    for (walker_id = 1; walker_id < PHARAOH_WALKER_RECORD_COUNT;
         ++walker_id) {
        const PharaohWalkerQueryRecord *walker = &context->walkers[walker_id];
        if (walker->state != 0 && walker->type ==
            PHARAOH_TRANSPORT_SHIP_WALKER_TYPE &&
            walker->ship_formation_id_092 == formation_id) {
            *context->current_walker_id = walker_id;
            return walker_id;
        }
    }
    *context->current_walker_id = PHARAOH_WALKER_RECORD_COUNT;
    return 0;
}
