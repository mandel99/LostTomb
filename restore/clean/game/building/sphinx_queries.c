#include "pharaoh/building/sphinx_queries.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohSphinxWorkSpot) == 0x10 &&
    offsetof(PharaohSphinxWorkSpot, stage) == 0 &&
    offsetof(PharaohSphinxWorkSpot, walker_type) == 4,
    "native Sphinx work-spot layout");

/* Complete C translation of Pharaoh.exe 0x00420600. */
void pharaoh_sphinx_get_work_percentages(
    const PharaohSphinxWorkSpot spots[PHARAOH_SPHINX_WORK_SPOT_COUNT],
    int16_t current_stage,
    int32_t *carpenter_percent, int32_t *mason_percent,
    int32_t *carpenters, int32_t *carpenters_done,
    int32_t *masons, int32_t *masons_done)
{
    *masons = 0;
    *carpenters = 0;
    *masons_done = 0;
    *carpenters_done = 0;
    for (int32_t index = 0; index < PHARAOH_SPHINX_WORK_SPOT_COUNT; ++index) {
        if (spots[index].walker_type == PHARAOH_WALKER_CARPENTER) {
            ++*carpenters;
            if (spots[index].stage < current_stage) ++*carpenters_done;
        } else {
            ++*masons;
            if (spots[index].stage < current_stage) ++*masons_done;
        }
    }
    *carpenter_percent =
        (int32_t)((uint32_t)*carpenters_done * 100u) / *carpenters;
    *mason_percent = (int32_t)((uint32_t)*masons_done * 100u) / *masons;
}
