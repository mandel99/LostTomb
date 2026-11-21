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

typedef struct PharaohMissionReplayState {
    int32_t scenario_mode;
    uint8_t *game_is_replay;
    const char *scenario_filename;
    const char *player_save_path;
    uint32_t *pending_load;
    uint8_t *briefing_flag;
    uint32_t *selection_flag;
    uint8_t *selection_byte;
    uint8_t *current_window_id;
    uint8_t *redraw_requested;
} PharaohMissionReplayState;

typedef struct PharaohMissionReplayOperations {
    void *user_data;
    void *(*open_file)(void *user_data, const char *path,
        const char *mode);
    void (*close_file)(void *user_data, void *file);
    void (*load_saved_game)(void *user_data, const char *path);
    void (*show_main_menu)(void *user_data);
    void (*begin_briefing)(void *user_data);
    void (*load_custom_map)(void *user_data, const char *filename,
        int32_t mode);
} PharaohMissionReplayOperations;

/* Complete typed translation of native 0x00418510. Ghidra has no function
   object at this dispatcher jump target; every instruction through its RET at
   0x00418632 was verified with the REA/Ghidra listing decoder. */
void pharaoh_mission_replay(PharaohMissionReplayState *state,
    const PharaohMissionReplayOperations *operations);

typedef struct PharaohMissionAdvanceState {
    PharaohCampaignIndex *index;
    PharaohSettingsFile *settings;
    uint32_t current_city_value_a;
    uint32_t current_city_value_b;
    uint32_t *last_city_value_a;
    uint32_t *last_city_value_b;
    uint32_t *mission_run_count;
    int32_t scenario_mode;
    uint8_t *game_is_replay;
    uint32_t *city_end_values[3];
    uint32_t *selection_flag;
    uint8_t *selection_byte;
    uint32_t *map_campaign_mode;
    uint32_t *map_mode;
    uint8_t *selected_choice;
    uint32_t *latest_period;
    uint32_t *selected_period;
    uint32_t *campaign_menu_mode;
    uint8_t *current_window_id;
    uint8_t *redraw_requested;
    uint32_t *simulation_flag_a;
    uint32_t *simulation_flag_b;
    int32_t screen_width;
    int32_t screen_height;
} PharaohMissionAdvanceState;

typedef struct PharaohMissionAdvanceOperations {
    void *user_data;
    void (*save_profile)(void *user_data);
    void (*store_mission_records)(void *user_data);
    void (*update_special_marker)(void *user_data);
    void (*show_map_selection)(void *user_data);
    void (*begin_briefing)(void *user_data);
    void (*play_video)(void *user_data, const char *path,
        int32_t x, int32_t y);
    void (*show_main_menu)(void *user_data);
    void (*selection_failure)(void *user_data, uint32_t origin);
} PharaohMissionAdvanceOperations;

/* Complete typed translation of native 0x00418640. selection_failure must not
   return when the native campaign-index invariant is violated. */
void pharaoh_mission_advance(PharaohMissionAdvanceState *state,
    const PharaohMissionAdvanceOperations *operations);

#endif
