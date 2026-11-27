#include "pharaoh/monuments/laborer_jobs.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohRoyalTombLaborerRecord) ==
    PHARAOH_BUILDING_RECORD_SIZE &&
    offsetof(PharaohRoyalTombLaborerRecord, stage) == 0x60 &&
    offsetof(PharaohRoyalTombLaborerRecord, can_take_workers) == 0x64 &&
    offsetof(PharaohRoyalTombLaborerRecord, chamber_kind) == 0x68 &&
    offsetof(PharaohRoyalTombLaborerRecord, remaining_work) == 0x84,
    "Native royal-tomb work query offsets");
_Static_assert(offsetof(PharaohMonumentSpotAnimation, work_remaining) == 0x0c &&
    offsetof(PharaohMonumentSpotAnimation, walker_id) == 0x12,
    "Native monument work-object offsets");
_Static_assert(offsetof(PharaohBuildingTypeRecord, state) == 0x00 &&
    offsetof(PharaohBuildingTypeRecord, grid_offset) == 0x0c &&
    offsetof(PharaohBuildingTypeRecord, building_type) == 0x10 &&
    offsetof(PharaohBuildingTypeRecord, road_access_x) == 0x26 &&
    offsetof(PharaohBuildingTypeRecord, road_access_y) == 0x28,
    "Native Sphinx destination-source offsets");
_Static_assert(offsetof(PharaohWalkerQueryRecord, type) == 0x0a &&
    offsetof(PharaohWalkerQueryRecord, destination_x) == 0x24 &&
    offsetof(PharaohWalkerQueryRecord, destination_y) == 0x26 &&
    offsetof(PharaohWalkerQueryRecord, destination_grid_offset) == 0x28 &&
    offsetof(PharaohWalkerQueryRecord, route_progress) == 0x3c,
    "Native Sphinx destination-write offsets");

/* Complete leaf C owner of native 0x00420350. Reuses the ordinary C loop from
   runtime/CarpenterJobs.c, qualified against all 69 pinned native instructions.
   The work scan does not reserve the free spot. */
int32_t pharaoh_sphinx_find_work_for_walker(
    const PharaohSphinxWorkEntry entries[PHARAOH_SPHINX_WORK_SPOT_COUNT],
    int16_t current_stage,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int32_t walker_id)
{
    PharaohWalkerQueryRecord *walker = &walkers[walker_id];
    int32_t index;
    int32_t building_id;

    for (index = 0; index < PHARAOH_SPHINX_WORK_SPOT_COUNT; ++index) {
        const PharaohSphinxWorkEntry *entry = &entries[index];
        if (entry->spot->walker_id == 0 &&
            entry->stage == (int32_t)current_stage &&
            entry->walker_type == (int32_t)walker->type &&
            entry->spot->work_remaining > 0) {
            break;
        }
    }
    if (index == PHARAOH_SPHINX_WORK_SPOT_COUNT) return 0;

    /* Native type-pointer interval 799AF0..89B6E8 selects IDs 1..3999. */
    for (building_id = 1; building_id < PHARAOH_BUILDING_RECORD_COUNT;
        ++building_id) {
        const PharaohBuildingTypeRecord *building = &buildings[building_id];
        if (building->state == 1 && building->building_type == 210) {
            int16_t destination_y;
            int32_t destination_grid_offset;

            walker->destination_x = building->road_access_x;
            destination_y = building->road_access_y;
            destination_grid_offset = building->grid_offset;
            walker->destination_y = destination_y;
            walker->destination_grid_offset = destination_grid_offset;
            walker->route_progress = 0;
            return building_id;
        }
    }
    return 0;
}

/* The native jump table at 423374 selects three-row descriptors beginning at
   5BE508 + table*48. This is the existing royal-tomb chamber-kind rule, checked
   against the pinned executable's complete 30-entry jump table. */
static const int8_t royal_tomb_chamber_work_table[
    PHARAOH_ROYAL_TOMB_LABORER_CHAMBER_KIND_COUNT] = {
     5,  0,  1,  2, 12, 13,  6, 14, 15,  7,
     8, 19, 16, 20, 21,  3,  4, -1, -1,  9,
    10, 11, 17, 18, 22, 23, 24, -1, -1,  0
};

/* Complete leaf C owner of native 0x00422A90. The ordinary C chamber dispatch
   and work predicate from MonumentManagerRoyalTombLogical.c replace the live
   IL wrapper. All 25 native branches have the same three-row search. */
int32_t pharaoh_royal_tomb_part_find_spot(
    const PharaohRoyalTombLaborerRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    const PharaohMonumentWorkEntry entries[
        PHARAOH_ROYAL_TOMB_LABORER_ENTRY_COUNT],
    int32_t building_id, int32_t walker_id)
{
    const PharaohRoyalTombLaborerRecord *building = &buildings[building_id];
    int32_t stage = building->stage;
    int32_t table;
    int32_t index;

    if (building->can_take_workers == 0) return 0;
    if ((uint32_t)building->chamber_kind >=
        PHARAOH_ROYAL_TOMB_LABORER_CHAMBER_KIND_COUNT) return 0;
    table = royal_tomb_chamber_work_table[building->chamber_kind];
    if (table < 0) return 0;

    for (index = 0; index < PHARAOH_ROYAL_TOMB_LABORER_ROWS_PER_TABLE; ++index) {
        const PharaohMonumentWorkEntry *entry = &entries[
            table * PHARAOH_ROYAL_TOMB_LABORER_ROWS_PER_TABLE + index];
        if (entry->spot->walker_id == 0 && entry->stage == stage &&
            entry->walker_type == (int32_t)walkers[walker_id].type &&
            building->remaining_work[stage - 1] > 0) {
            return building_id;
        }
    }
    return 0;
}
