#ifndef PHARAOH_SCENARIO_SAVED_SCENE_STORAGE_H
#define PHARAOH_SCENARIO_SAVED_SCENE_STORAGE_H

#include "pharaoh/scenario/saved_game_io.h"
#include "pharaoh/scenario/saved_history.h"
#include "pharaoh/scenario/mission_records.h"
#include "pharaoh/scenario/saved_game_upgrades.h"
#include "pharaoh/runtime/game_file_upgrade.h"

enum { PHARAOH_SAVED_SCENE_CHUNKS = 181 };

typedef struct PharaohSavedSceneMemory {
    void *user_data;
    void *(*allocate)(void *user_data, uint32_t bytes);
    void (*release)(void *user_data, void *allocation);
} PharaohSavedSceneMemory;

/* Views of established native chunk layouts. Other payloads remain opaque
 * serialized bytes until their consumers provide a qualified type. */
typedef struct PharaohSavedSceneViews {
    uint32_t *images, *terrain, *service_visits;
    uint8_t *edges, *properties, *canal_water, *random;
    uint8_t *variation_tag, *meadow_value, *fertility;
    int8_t *height_values;
    int16_t *building_ids, *walker_ids;
    PharaohWalkerQueryRecord *walkers;
    PharaohBuildingTypeRecord *buildings;
    PharaohScenarioPlayerRuntime *players;
    int32_t *current_player, *city_orientation;
    PharaohEditorRoute *routes;
    PharaohUpgradeStorageRecord *storages;
    PharaohUpgradeEmpireCityRecord *empire_cities;
    PharaohUpgradeEmpireObjectRecord *empire_objects;
    PharaohSavedHistory *history_snapshots;
    PharaohFloodplainSettings *floodplain_settings;
    PharaohMonumentSpotAnimation *work_spots[4];
    uint8_t *metadata, *game_is_replay;
} PharaohSavedSceneViews;

typedef struct PharaohSavedSceneStorage {
    PharaohSavedSceneMemory memory;
    void *allocation;
    uint32_t allocation_bytes;
    void *destinations[PHARAOH_SAVED_SCENE_CHUNKS];
    PharaohSettingsFile *settings;
    PharaohSavedSceneViews view;
    /* Chunk 175 is the first 12 bytes of started, inside this live owner. */
    PharaohMissionRecordsState mission_records;
    /* Derived native global not serialized in the 181 payload slots. */
    uint8_t pyramid_speedup_active;
} PharaohSavedSceneStorage;

/* Open an empty owner once; close it before reuse. NULL memory selects the
 * CRT heap. The allocator must provide normal C allocation alignment.
 * Chunks 0..3 and 174 alias live settings. Headers 4..6 belong to the IO context
 * and are NULL until bind_reader. No serialized process pointer is followed. */
int32_t pharaoh_saved_scene_storage_open(PharaohSavedSceneStorage *storage,
    PharaohSettingsFile *settings, const PharaohSavedSceneMemory *memory);

/* Release route samples, histories and other child allocations with their
 * own C owners first. This releases only the payload arena, not those children
 * or the borrowed settings/IO contexts. */
void pharaoh_saved_scene_storage_close(PharaohSavedSceneStorage *storage);

/* Connect the real 181-chunk IO reader to these live buffers. Required read
 * operations (upgrades, routes, work spots) remain caller-owned and intact. */
void pharaoh_saved_scene_bind_reader(PharaohSavedSceneStorage *storage,
    PharaohSavedGameIo *io, PharaohSavedGameReadContext *read,
    const PharaohChunkMigrationRecord *message_migrations,
    const PharaohChunkMigrationRecord *event_migrations,
    const PharaohChunkIoContext *external,
    const PharaohCompressedChunkState *compressed);

/* Bind the upgrade buffers owned by this scene, including chunk 175's parent.
 * Other inputs (scripts, dynamic histories, constants and current-player
 * dependent state) remain caller-owned. The read context must be bound too. */
void pharaoh_saved_scene_bind_upgrades(PharaohSavedSceneStorage *storage,
    PharaohSavedGameUpgradeBindings *bindings);

#endif
