#include "pharaoh/scenario/scenario_start_mission.h"
#include "pharaoh/core/percentage_adjust.h"

#include <string.h>

_Static_assert(sizeof(void *) != 4 || sizeof(PharaohTutorialRule) == 28,
    "native tutorial-rule stride changed");
_Static_assert(offsetof(PharaohScenarioPlayerRuntime, salary) == 0x2c98 &&
    offsetof(PharaohScenarioPlayerRuntime, rank_clamped) == 0x2c9c &&
    offsetof(PharaohScenarioPlayerRuntime, starting_favor) == 0x42d8 &&
    offsetof(PharaohScenarioPlayerRuntime, initial_rank) == 0x444c &&
    offsetof(PharaohScenarioPlayerRuntime, personal_savings) == 0x4450 &&
    offsetof(PharaohScenarioPlayerRuntime, tutorial_active_a) == 0x4720 &&
    offsetof(PharaohScenarioPlayerRuntime, tutorial_active_b) == 0x4724 &&
    offsetof(PharaohScenarioPlayerRuntime, difficulty) == 0x47a8,
    "native scenario-start player offsets changed");

void pharaoh_tutorial_mission_menu_flags(int32_t mission_id,
    int32_t advisor_access_override, const PharaohTutorialRule *rules,
    const uint8_t *tutorial_flags_a, uint8_t *tutorial_flags_b)
{
    if (mission_id > PHARAOH_TUTORIAL_LAST_MISSION ||
        advisor_access_override != 0) {
        for (int32_t flag = 0; flag < PHARAOH_TUTORIAL_MENU_FLAG_COUNT;
             ++flag) {
            tutorial_flags_b[flag] = 1;
        }
        return;
    }

    for (int32_t tutorial = 0; tutorial < PHARAOH_TUTORIAL_RULE_COUNT;
         ++tutorial) {
        const PharaohTutorialRule *const rule = &rules[tutorial];
        if (rule->mission_threshold != mission_id) continue;

        for (int32_t index = 0; index < rule->item_count; ++index) {
            tutorial_flags_b[rule->items[index]] =
                tutorial_flags_a[tutorial] != 0;
        }
    }
}

static uint32_t copy_terminated(char *destination, const char *source)
{
    uint32_t size = 0;
    do {
        destination[size] = source[size];
        ++size;
    } while (destination[size - 1] != '\0');
    return size;
}

static void enable_tutorial_rules(PharaohScenarioStartMissionContext *context,
    int8_t mission)
{
    uint32_t rule_index;
    memset(context->tutorial_flags_a, 0, context->tutorial_flags_a_count);
    memset(context->tutorial_flags_b, 0, context->tutorial_flags_b_count);

    for (rule_index = 0; rule_index < context->tutorial_rule_count;
         ++rule_index) {
        const PharaohTutorialRule *rule = &context->tutorial_rules[rule_index];
        const uint8_t enabled = mission > rule->mission_threshold;
        int32_t item;
        context->tutorial_flags_a[rule_index] = enabled;
        for (item = 0; item < rule->item_count; ++item)
            context->tutorial_flags_b[rule->items[item]] = enabled;
    }
}

/* Complete typed translation of Pharaoh.exe 0x005405B0. */
void pharaoh_scenario_start_mission(
    PharaohScenarioStartMissionContext *context, const char *name,
    char force_campaign)
{
    PharaohSettingsFile *settings = context->settings;
    void *const steps = context->steps_user_data != NULL ?
        context->steps_user_data : context->user_data;
    char path[100];
    int32_t saved_personal_savings = settings->personal_savings_last_mission;
    uint8_t saved_current_mission = (uint8_t)settings->current_mission_id;
    int8_t saved_mission = settings->mission_id;
    uint8_t saved_campaign = settings->campaign;
    uint8_t saved_previous_link = settings->previous_campaign_link;
    int8_t saved_campaign_link = settings->campaign_link;
    uint8_t saved_replay = *context->game_is_replay;
    int32_t loaded_custom;
    int32_t player_index;
    PharaohScenarioPlayerRuntime *player;
    int32_t rank;
    uint32_t index;

    copy_terminated(path, context->path_prefix);
    copy_terminated(path + strlen(path), name);
    *context->tutorial_legions_message_posted = 1;
    context->reset_native_pairs(steps);

    if (settings->is_custom_scenario != 0 && force_campaign == 0) {
        if (!context->map_exists(context->user_data, path)) {
            *context->current_window_id = 1;
            *context->window_redraw_requested = 1;
            return;
        }
        context->setup_new_game(context->user_data,path);
    } else {
        copy_terminated(context->player_name_destination,
            settings->player_name);
        if (!context->load_campaign_mission(context->campaign_load_user_data != NULL ?
                context->campaign_load_user_data : context->user_data, 0, 0))
            return;
        settings->is_custom_scenario = 0;
        player = &context->players[*context->current_player];
        player->treasury = pharaoh_adjust_by_percentage(player->treasury,
            context->difficulty_percentages[settings->difficulty]);
        copy_terminated(context->player_name_destination,
            settings->player_name);
    }

    settings->current_mission_id = saved_current_mission;
    *context->game_is_replay = saved_replay;
    settings->mission_id = saved_mission;
    settings->campaign = saved_campaign;
    settings->personal_savings_last_mission = saved_personal_savings;
    if (saved_replay == 3) {
        saved_campaign_link = (int8_t)pharaoh_campaign_link_of_mission(
            context->campaign_index, saved_mission);
        if (saved_campaign_link < 0) saved_campaign_link = 0;
        saved_previous_link = UINT8_C(0xff);
    }
    settings->campaign_link = saved_campaign_link;
    settings->previous_campaign_link = saved_previous_link;

    player_index = *context->current_player;
    player = &context->players[player_index];
    loaded_custom = settings->is_custom_scenario;
    settings->starting_favor =
        context->starting_favor_by_difficulty[settings->difficulty];
    player->starting_favor = settings->starting_favor;
    player->personal_savings = settings->personal_savings_last_mission;
    if (loaded_custom != 0) player->personal_savings = 0;

    rank = *context->scenario_initial_rank;
    player->initial_rank = rank;
    player->rank_clamped = rank;
    if (rank > 10) player->rank_clamped = 10;
    *context->tutorial_first_mission = 0;
    *context->tutorial_first_five = 0;
    player->salary = context->salary_by_rank[player->rank_clamped];
    player->tutorial_active_a = 1;
    for (index = 0; index < 7; ++index)
        *context->tutorial_ready_flags[index] = 1;
    player->tutorial_active_b = 1;

    if (loaded_custom == 0) {
        if (settings->mission_id == 0) *context->tutorial_first_mission = 1;
        if (settings->mission_id <= 4) *context->tutorial_first_five = 1;
    }

    if (settings->mission_id <= 10 && loaded_custom == 0) {
        enable_tutorial_rules(context, settings->mission_id);
    } else {
        memset(context->tutorial_flags_a, 1,
            context->tutorial_flags_a_count);
        memset(context->tutorial_flags_b, 1,
            context->tutorial_flags_b_count);
    }

    for (index = 0; index < 4; ++index)
        context->final_steps[index](steps);

    player = &context->players[*context->current_player];
    if (settings->is_custom_scenario == 0) {
        player->starting_favor = settings->starting_favor;
        player->personal_savings = settings->personal_savings_last_mission;
    }
    player->difficulty = settings->difficulty;
    *context->scenario_animation_flag =
        (uint8_t)(*context->scenario_animation_flag == 1);
    context->initialize_walker_actions(steps);
}
