#include "pharaoh/scenario/mission_records.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohMissionLegionRecord) == 12,
    "mission legion record size");
_Static_assert(sizeof(PharaohMissionCarryRecords) == 40,
    "mission carry record size");
_Static_assert(sizeof(PharaohMissionRecordsState) == 80 &&
    offsetof(PharaohMissionRecordsState, started) == 0x28,
    "mission records state layout");

static void reset_legions(PharaohMissionCarryRecords *records)
{
    for (int32_t i = 0; i < PHARAOH_MISSION_LEGION_KINDS; ++i) {
        records->legions[i].legion_id = PHARAOH_MISSION_LEGION_NONE;
        records->legions[i].experience = -1;
        records->legions[i].soldiers = 0;
    }
}

/* Complete C translation of Pharaoh.exe 0x004D3C40. */
void pharaoh_mission_records_reset(PharaohMissionRecordsState *state)
{
    state->started.savings = 0;
    reset_legions(&state->started);
    reset_legions(&state->finished);
}
