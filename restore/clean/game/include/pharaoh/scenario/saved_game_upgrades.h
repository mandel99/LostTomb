#ifndef PHARAOH_SCENARIO_SAVED_GAME_UPGRADES_H
#define PHARAOH_SCENARIO_SAVED_GAME_UPGRADES_H

#include "pharaoh/scenario/saved_game_state_read.h"
#include "pharaoh/scenario/saved_history.h"
#include "pharaoh/scenario/saved_walker_names.h"
#include "pharaoh/scenario/saved_event_upgrade.h"
#include "pharaoh/runtime/game_file_upgrade.h"
#include "pharaoh/core/static_objects.h"

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
} PharaohSavedGameUpgradeBindings;

/* Binds real C children of004D41E0 without replacing the route/work-spot
 * contexts. V7C_1AD0 and V88 retain their separately required bindings. */
void pharaoh_saved_game_bind_upgrade_steps(PharaohSavedGameReadOperations *ops,
    PharaohSavedGameUpgradeBindings *bindings);

#endif
