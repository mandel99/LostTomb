#ifndef PHARAOH_MONUMENTS_LABORER_JOBS_H
#define PHARAOH_MONUMENTS_LABORER_JOBS_H

#include "pharaoh/building/type_queries.h"
#include "pharaoh/monuments/monument_work_spots.h"
#include "pharaoh/monuments/sphinx_work.h"

#include <stdint.h>

enum {
    PHARAOH_ROYAL_TOMB_LABORER_TABLE_COUNT = 25,
    PHARAOH_ROYAL_TOMB_LABORER_ROWS_PER_TABLE = 3,
    PHARAOH_ROYAL_TOMB_LABORER_ENTRY_COUNT = 75,
    PHARAOH_ROYAL_TOMB_LABORER_CHAMBER_KIND_COUNT = 30
};

/* Native royal-tomb building overlay. The availability field is a DWORD,
   including when its low byte is zero. The four work counters begin at 0x84;
   the pinned work descriptors select stages 1, 2, 3 only. */
typedef struct PharaohRoyalTombLaborerRecord {
    uint8_t reserved_000[0x60];
    int32_t stage;
    int32_t can_take_workers;
    int32_t chamber_kind;
    uint8_t reserved_06c[0x84 - 0x6c];
    int32_t remaining_work[4];
    uint8_t reserved_094[PHARAOH_BUILDING_RECORD_SIZE - 0x94];
} PharaohRoyalTombLaborerRecord;

/* Native 0x00420350 ignores its first two scalar arguments. current_stage is the
   signed WORD selected from the current player's record. Entries retain the
   native 22-row order; buildings and walkers retain their native ID indexing. */
int32_t pharaoh_sphinx_find_work_for_walker(
    const PharaohSphinxWorkEntry entries[PHARAOH_SPHINX_WORK_SPOT_COUNT],
    int16_t current_stage,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int32_t walker_id);

/* Native 0x00422A90 reads the complete 75-entry table, grouped in 25 tables of 3.
   Its descriptor stages must retain the pinned domain 1..3. No spot is claimed
   and no destination is changed; the full result is building_id or zero. */
int32_t pharaoh_royal_tomb_part_find_spot(
    const PharaohRoyalTombLaborerRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    const PharaohMonumentWorkEntry entries[
        PHARAOH_ROYAL_TOMB_LABORER_ENTRY_COUNT],
    int32_t building_id, int32_t walker_id);

#endif
