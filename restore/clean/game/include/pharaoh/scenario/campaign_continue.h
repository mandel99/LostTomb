#ifndef PHARAOH_SCENARIO_CAMPAIGN_CONTINUE_H
#define PHARAOH_SCENARIO_CAMPAIGN_CONTINUE_H

#include "pharaoh/runtime/settings_ini.h"
#include "pharaoh/scenario/campaign_start.h"

#include <stdint.h>

typedef struct PharaohCampaignContinueState {
    const PharaohCampaignIndex *index;
    PharaohSettingsFile *settings;
    uint8_t *current_window_id;
    uint8_t *transition_mode;
    uint8_t *redraw_requested;
    uint32_t *selected_choice;
    int32_t *focus_ids[4];
    int32_t *previous_city_overlay;
    uint32_t *city_state_word;
    int32_t *mission_record_count;
    int32_t mission_record_limit;
    int32_t *special_music_playing;
} PharaohCampaignContinueState;

typedef struct PharaohCampaignContinueOperations {
    void *user_data;
    void (*create_player)(void *user_data);
    void (*store_mission_records)(void *user_data);
    void (*begin_briefing)(void *user_data);
    void (*prepare_start)(void *user_data, int32_t value);
    void (*selection_failure)(void *user_data, uint32_t origin);
} PharaohCampaignContinueOperations;

/* Complete typed translation of native 0x00418220 for its reachable state
   domain. selection_failure must not return when the campaign invariant is
   violated, matching the terminal guard in the old verified C owner. */
void pharaoh_campaign_continue(PharaohCampaignContinueState *state,
    const PharaohCampaignContinueOperations *operations);

typedef struct PharaohMissionReplayAvailabilityState {
    int32_t scenario_mode;
    uint8_t game_is_replay;
    const char *scenario_filename;
    const char *player_save_path;
} PharaohMissionReplayAvailabilityState;

typedef struct PharaohMissionReplayAvailabilityOperations {
    void *user_data;
    int32_t (*custom_map_replay_available)(void *user_data,
        const char *map_path);
    void *(*open_file)(void *user_data, const char *path);
    void (*close_file)(void *user_data, void *file);
} PharaohMissionReplayAvailabilityOperations;

/* Complete typed translation of native 0x004183D0. Native inputs guarantee
   that the assembled path fits its 100-byte stack buffer. */
uint8_t pharaoh_mission_replay_available(
    const PharaohMissionReplayAvailabilityState *state,
    const PharaohMissionReplayAvailabilityOperations *operations);

#endif
