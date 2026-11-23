#ifndef PHARAOH_SCENARIO_SAVED_GAME_UPGRADES_H
#define PHARAOH_SCENARIO_SAVED_GAME_UPGRADES_H

#include "pharaoh/scenario/saved_game_state_read.h"
#include "pharaoh/scenario/saved_history.h"
#include "pharaoh/scenario/saved_walker_names.h"
#include "pharaoh/scenario/saved_event_upgrade.h"
#include "pharaoh/runtime/game_file_upgrade.h"
#include "pharaoh/core/static_objects.h"
#include "pharaoh/scenario/player_relayout.h"
#include "pharaoh/monuments/obelisk_queries.h"
#include "pharaoh/scenario/mission_records.h"

typedef struct PharaohSavedGameUpgradeBindings {
    PharaohSavedGameReadContext *read;
    PharaohWalkerQueryRecord *walkers;
    PharaohFormationTotalsRecord *formations;
    PharaohWalkerNameCounters *name_counters;
    PharaohUpgradeEmpireCityRecord *empire_cities;
    PharaohUpgradeEmpireObjectRecord *empire_objects;
    PharaohUpgradeStorageRecord *storages;
    PharaohNativeRecord30 *messages;
    PharaohScenarioEvent *events;
    const int16_t *event_count;
    int32_t (*resource_prices)[2]; /* resources31..35 */
    const int32_t (*default_resource_prices)[2];
    const int32_t *pyramid_style, *pyramid_parts, *mastaba_parts;
    int32_t *sphinx_completed;
    int32_t (*saved_pairs)[2]; /* four native pairs */
    PharaohDynamicQueueSlot *histories;
    PharaohSavedHistory *history_snapshots;
    const PharaohHistoryMemory *history_memory;
    const PharaohMonumentWorkSpotBinding *small_obelisk_spots, *large_obelisk_spots;
    const PharaohMonumentAnimScript *monument_scripts; /*253 native records */
    PharaohCrtRandom *crt_random;
    PharaohMissionRecordsState *mission_records;
} PharaohSavedGameUpgradeBindings;

/* Binds all27 upgrade children of004D41E0 without replacing the route/work-spot
 * contexts. The caller owns the buffers, scripts, spot bindings and CRT seed. */
void pharaoh_saved_game_bind_upgrade_steps(PharaohSavedGameReadOperations *ops,
    PharaohSavedGameUpgradeBindings *bindings);
void pharaoh_game_file_upgrade_obelisk_work_spots(PharaohSavedGameUpgradeBindings *bindings);

typedef struct PharaohSavedGameSpotBindings {
    const PharaohMonumentWorkSpotBinding *caesareum, *lighthouse, *library, *abu_simbel;
    const PharaohMonumentWorkSpotGroup *royal_tombs; /*25 groups of3 */
    const PharaohMonumentAnimScript *scripts; /*253 records */
    PharaohCrtRandom *random;
} PharaohSavedGameSpotBindings;

/* The five absence repairs at the end of004D41E0. */
void pharaoh_saved_game_bind_work_spot_steps(PharaohSavedGameReadOperations *ops,
    PharaohSavedGameSpotBindings *bindings);

#endif
