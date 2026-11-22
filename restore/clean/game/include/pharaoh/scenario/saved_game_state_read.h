#ifndef PHARAOH_SCENARIO_SAVED_GAME_STATE_READ_H
#define PHARAOH_SCENARIO_SAVED_GAME_STATE_READ_H

#include "pharaoh/scenario/scenario_state_read.h"
#include "pharaoh/scenario/scenario_init_custom.h"
#include "pharaoh/runtime/settings_ini.h"
#include "pharaoh/building/type_queries.h"
#include "pharaoh/rendering/monument_spot_animation.h"
#include "pharaoh/simulation/flood_settings.h"

typedef void (*PharaohSavedGameStep)(void *user_data);

/* Ordered legacy calls in native 0x004D41E0. Slots own typed C adapters,
 * never native-address calls or a fallback to an IL interpreter. */
enum PharaohSavedGameUpgrade {
    PHARAOH_SAVE_V76,       /* 004D17E0 */
    PHARAOH_SAVE_V78,       /* 004D1840 */
    PHARAOH_SAVE_V79,       /* 004D1860 */
    PHARAOH_SAVE_V7A,       /* 004D1880 */
    PHARAOH_SAVE_V7B,       /* 004D18B0 */
    PHARAOH_SAVE_V7C_CITIES, /* 004D18F0 */
    PHARAOH_SAVE_V7C_1AD0,
    PHARAOH_SAVE_V7C_BUILDINGS, /* 004D2FA0 */
    PHARAOH_SAVE_V7C_STORAGES,  /* 004D3200 */
    PHARAOH_SAVE_V7F,       /* 004D32D0 */
    PHARAOH_SAVE_V81,       /* 004D32A0 */
    PHARAOH_SAVE_ALWAYS_32F0,
    PHARAOH_SAVE_V88,       /* 004D3C20 */
    PHARAOH_SAVE_V8B,       /* 004D3CC0 */
    PHARAOH_SAVE_V8F,       /* 004DE140 */
    PHARAOH_SAVE_V90,       /* 004D3D80 */
    PHARAOH_SAVE_V92_MONUMENTS, /* 004D3DA0 */
    PHARAOH_SAVE_V92_FORMATIONS, /* 004D3F40 */
    PHARAOH_SAVE_V94,       /* 004D3D00 */
    PHARAOH_SAVE_V95,       /* 004D3F60 */
    PHARAOH_SAVE_V96,       /* 004D4040 */
    PHARAOH_SAVE_V98,       /* 004D40C0 */
    PHARAOH_SAVE_V99,       /* 004D19E0 */
    PHARAOH_SAVE_V9B_BUILDINGS, /* 004D4080 */
    PHARAOH_SAVE_V9B_WORDS, /* 004D40A0 */
    PHARAOH_SAVE_V9C,       /* 004D1A70 */
    PHARAOH_SAVE_VA0,       /* 004D4180 */
    PHARAOH_SAVE_UPGRADE_COUNT
};

typedef struct PharaohSavedGameReadOperations {
    void *user_data;
    PharaohScenarioRouteFree free_route_samples;
    PharaohScenarioRouteRebuild rebuild_route;
    PharaohSavedGameStep reset_mission_records; /* 004D3C40 */
    void (*apply_loaded_settings)(void *user_data, int32_t mode); /* 004D0A40 */
    PharaohSavedGameStep upgrade[PHARAOH_SAVE_UPGRADE_COUNT];
    /* Caesareum, lighthouse, library, Abu Simbel, royal tombs. */
    PharaohSavedGameStep reset_work_spots[5];
} PharaohSavedGameReadOperations;

typedef struct PharaohSavedGameReadContext {
    PharaohChunkIoContext *chunk_io;
    PharaohChunkDescriptor *descriptors;
    PharaohEditorRoute *routes; /* 50 native records */
    PharaohBuildingTypeRecord *buildings; /* 4000 native records */
    PharaohMonumentSpotAnimation *work_spots[4]; /* 10,18,22,13 records */
    PharaohSettingsFile *settings;
    PharaohScenarioPlayerRuntime *players;
    const int32_t *current_player;
    uint16_t *allowed_buildings; /* 100 words; first50 default to1 */
    uint16_t *predator_variant;
    void *service_visit_grid; /* 0x32c40 bytes */
    uint8_t *game_is_replay;
    PharaohFloodplainSettings *floodplain_settings;
    void *allowed_buildings_tail; /* 110 bytes from native 00784D3A */
    int32_t *map_sweep_countdown;
    int32_t *frogs_countdown;
    int32_t *pyramid_speedup_countdown;
    int32_t *map_sweep_total;
    int32_t *locusts_countdown;
    int32_t *mummies_max;
    int32_t *hailstorm_countdown;
    uint8_t *animation_set_bytes; /* two separately written bytes */
    uint8_t *pyramid_speedup_active;
    const PharaohSavedGameReadOperations *operations;
} PharaohSavedGameReadContext;

/* Complete typed C reuse of Unit01821.c, native 0x004D41E0.
 * Full saved-state reader; distinct from map-only 0x004D4AE0. */
void pharaoh_saved_game_state_read(PharaohSavedGameReadContext *context,
    int32_t file);

#endif
