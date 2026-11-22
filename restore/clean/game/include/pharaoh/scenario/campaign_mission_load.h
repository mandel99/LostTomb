#ifndef PHARAOH_SCENARIO_CAMPAIGN_MISSION_LOAD_H
#define PHARAOH_SCENARIO_CAMPAIGN_MISSION_LOAD_H

#include "pharaoh/runtime/file_read_part.h"
#include "pharaoh/runtime/settings_ini.h"
#include "pharaoh/scenario/saved_game_state_read.h"
#include "pharaoh/scenario/saved_game_initialize.h"
#include "pharaoh/simulation/flood_settings.h"

#include <stdint.h>

typedef struct PharaohCampaignMissionLoadOperations {
    void *user_data;
    void (*read_saved_game_state)(void *user_data, int32_t file_handle);
    void (*reset_floodplain_settings)(void *user_data);
    void (*initialize_saved_game)(void *user_data);
} PharaohCampaignMissionLoadOperations;

typedef struct PharaohCampaignMissionLoadContext {
    const PharaohFileReadPartContext *file_read_part;
    PharaohSettingsFile *settings;
    uint32_t *player_cheats_used;
    const PharaohCampaignMissionLoadOperations *operations;
} PharaohCampaignMissionLoadContext;

typedef struct PharaohCampaignMissionStateContext {
    PharaohSavedGameReadContext *read;
    PharaohSavedGameInitializeContext *initialize;
    PharaohFloodplainSettings *floodplain_settings;
} PharaohCampaignMissionStateContext;

/* Connect 004D58B0 to its actual C children004D41E0,004BD360,004D52A0.
 * The caller supplies full scene storage and typed initialization operations. */
void pharaoh_campaign_mission_bind_state(
    PharaohCampaignMissionLoadContext *load,
    PharaohCampaignMissionLoadOperations *operations,
    PharaohCampaignMissionStateContext *state);

/* Native entry 0x004D58B0. Loads the selected mission state from the root
   mission package and preserves the current difficulty across that load. */
int32_t pharaoh_campaign_mission_load(
    const PharaohCampaignMissionLoadContext *context, int32_t package_index,
    int32_t skip_initialize);

#endif
