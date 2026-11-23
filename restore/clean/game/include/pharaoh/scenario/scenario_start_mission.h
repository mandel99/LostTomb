#ifndef PHARAOH_SCENARIO_SCENARIO_START_MISSION_H
#define PHARAOH_SCENARIO_SCENARIO_START_MISSION_H

#include "pharaoh/runtime/settings_ini.h"
#include "pharaoh/scenario/campaign_start.h"
#include "pharaoh/scenario/tutorial_rules.h"
#include "pharaoh/scenario/scenario_init_custom.h"

#include <stdbool.h>
#include <stdint.h>



typedef void (*PharaohScenarioStartStep)(void *user_data);

typedef struct PharaohScenarioStartMissionContext {
    void *user_data;
    const char *path_prefix;
    PharaohSettingsFile *settings;
    uint8_t *game_is_replay;
    uint8_t *tutorial_legions_message_posted;
    PharaohScenarioStartStep reset_native_pairs;
    bool (*map_exists)(void *user_data, char *path);
    void (*setup_new_game)(void *user_data,char *path);
    int32_t (*load_campaign_mission)(void *user_data, int32_t package_index,
        int32_t skip_initialize);
    uint8_t *current_window_id;
    uint8_t *window_redraw_requested;
    char *player_name_destination;
    int32_t *current_player;
    PharaohScenarioPlayerRuntime *players;
    const int32_t *difficulty_percentages;
    const int32_t *starting_favor_by_difficulty;
    const int32_t *salary_by_rank;
    const int16_t *scenario_initial_rank;
    const PharaohCampaignIndex *campaign_index;
    uint32_t *tutorial_first_mission;
    uint32_t *tutorial_first_five;
    uint32_t *tutorial_ready_flags[7];
    const PharaohTutorialRule *tutorial_rules;
    uint32_t tutorial_rule_count;
    uint8_t *tutorial_flags_a;
    uint32_t tutorial_flags_a_count;
    uint8_t *tutorial_flags_b;
    uint32_t tutorial_flags_b_count;
    PharaohScenarioStartStep final_steps[4];
    uint8_t *scenario_animation_flag;
    PharaohScenarioStartStep initialize_walker_actions;
    void *campaign_load_user_data; /* NULL retains user_data. */
} PharaohScenarioStartMissionContext;

void pharaoh_scenario_start_mission(
    PharaohScenarioStartMissionContext *context, const char *name,
    char force_campaign);

#endif
