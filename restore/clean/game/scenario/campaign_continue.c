#include "pharaoh/scenario/campaign_continue.h"

enum {
    CAMPAIGN_CONTINUE_CITY_WINDOW = 0x01,
    CAMPAIGN_CONTINUE_CHOICE_WINDOW = 0x34,
    CAMPAIGN_CONTINUE_BRIEFING_WINDOW = 0x35,
    CAMPAIGN_CONTINUE_NAME_WINDOW = 0x36
};

static void request_redraw(PharaohCampaignContinueState *state)
{
    *state->redraw_requested = 1;
}

void pharaoh_campaign_continue(PharaohCampaignContinueState *state,
    const PharaohCampaignContinueOperations *operations)
{
    for (int32_t index = 0; index < 4; ++index) {
        *state->focus_ids[index] = 0;
    }
    *state->previous_city_overlay = 0;

    if (*state->current_window_id == CAMPAIGN_CONTINUE_NAME_WINDOW) {
        operations->create_player(operations->user_data);
        request_redraw(state);
        return;
    }

    if (*state->current_window_id == CAMPAIGN_CONTINUE_CHOICE_WINDOW) {
        if ((uint8_t)*state->selected_choice != 0) {
            operations->store_mission_records(operations->user_data);
            const int32_t previous = state->settings->campaign_link;
            state->settings->previous_campaign_link = (uint8_t)previous;
            const int32_t next = pharaoh_campaign_find_mission(state->index,
                (uint32_t)(int32_t)(int8_t)state->settings->campaign,
                previous, (int32_t)((*state->selected_choice & 0xffu) - 1u));
            state->settings->campaign_link = (int8_t)(uint8_t)next;
            const int32_t selected = state->settings->campaign_link;
            if (selected < 0 || selected >= state->index->count) {
                operations->selection_failure(operations->user_data,
                    UINT32_C(0x00418220));
                return;
            }
            state->settings->mission_id = (int8_t)
                state->index->records[selected].link.mission_id;
            operations->begin_briefing(operations->user_data);
            request_redraw(state);
            return;
        }
    } else {
        if (*state->current_window_id == CAMPAIGN_CONTINUE_BRIEFING_WINDOW) {
            *state->city_state_word = 0;
        }
        *state->current_window_id = CAMPAIGN_CONTINUE_CITY_WINDOW;
        *state->transition_mode = 2;
        operations->prepare_start(operations->user_data, 1);
        *state->special_music_playing = 0;
        if (*state->mission_record_count > state->mission_record_limit) {
            *state->mission_record_count = state->mission_record_limit;
        }
    }
    request_redraw(state);
}

static char *append_string(char *destination, const char *source)
{
    while (*destination != '\0') ++destination;
    while ((*destination++ = *source++) != '\0') { }
    return destination - 1;
}

uint8_t pharaoh_mission_replay_available(
    const PharaohMissionReplayAvailabilityState *state,
    const PharaohMissionReplayAvailabilityOperations *operations)
{
    char path[100] = { 0 };
    if (state->scenario_mode != 0) {
        append_string(path, "maps\\");
        append_string(path, state->scenario_filename);
        return operations->custom_map_replay_available(
            operations->user_data, path) != 0;
    }
    if (state->game_is_replay == 1) {
        append_string(path, state->player_save_path);
        append_string(path, "\\");
        append_string(path, "autosave_replay.sav");
        void *const file = operations->open_file(operations->user_data, path);
        if (file == 0) return 0;
        operations->close_file(operations->user_data, file);
    }
    return 1;
}
