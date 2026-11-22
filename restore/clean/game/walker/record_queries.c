#include "pharaoh/walker/record_queries.h"

void pharaoh_walker_save_type_78_sources(PharaohWalkerQueryRecord *walkers)
{
    int32_t id;
    for (id = 1; id < PHARAOH_WALKER_RECORD_COUNT; ++id) {
        PharaohWalkerQueryRecord *w = &walkers[id];
        if (w->type == 78 && w->state == 1 && w->action_state != 4) {
            const uint8_t *raw = (const uint8_t *)w;
            w->saved_source_x = *(const int16_t *)(raw + 0x14);
            w->saved_source_y = *(const int16_t *)(raw + 0x16);
        }
    }
}
