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

void pharaoh_mission_replay(PharaohMissionReplayState *state,
    const PharaohMissionReplayOperations *operations)
{
    char path[100] = { 0 };
    *state->pending_load = 0;

    if (state->scenario_mode == 0) {
        if (*state->game_is_replay != 1) {
            *state->briefing_flag = 0;
            operations->begin_briefing(operations->user_data);
            return;
        }

        append_string(path, state->player_save_path);
        append_string(path, "\\");
        append_string(path, "autosave_replay.sav");
        void *const file = operations->open_file(
            operations->user_data, path, "rb");
        if (file == 0) {
            operations->show_main_menu(operations->user_data);
            return;
        }
        operations->close_file(operations->user_data, file);
        *state->selection_flag = 0;
        *state->selection_byte = 0;
        operations->load_saved_game(operations->user_data, path);
    } else {
        *state->game_is_replay = 0;
        operations->load_custom_map(operations->user_data,
            state->scenario_filename, 0);
    }

    *state->current_window_id = 1;
    *state->redraw_requested = 1;
}

void pharaoh_mission_advance(PharaohMissionAdvanceState *state,
    const PharaohMissionAdvanceOperations *operations)
{
    int32_t next = -1;
    uint8_t choice_pending = 0;

    *state->last_city_value_a = state->current_city_value_a;
    *state->last_city_value_b = state->current_city_value_b;
    ++*state->mission_run_count;

    if (state->scenario_mode == 0) {
        const int32_t selected = state->settings->campaign_link;
        if (selected >= 0 && selected < state->index->count) {
            state->index->records[selected].link.played = 1;
            operations->save_profile(operations->user_data);
        }

        next = pharaoh_campaign_find_mission(state->index,
            (uint32_t)(int32_t)(int8_t)state->settings->campaign,
            state->settings->campaign_link, 0);
        if ((*state->game_is_replay == 2 || *state->game_is_replay == 1) &&
            next >= 0) {
            operations->store_mission_records(operations->user_data);
            const int32_t current = state->settings->campaign_link;
            if (current >= 0 && current < state->index->count) {
                if (state->index->records[current].link.choice_count < 2) {
                    state->settings->previous_campaign_link =
                        (uint8_t)state->settings->campaign_link;
                    state->settings->campaign_link = (int8_t)next;
                    if (next < 0 || next >= state->index->count) {
                        operations->selection_failure(operations->user_data,
                            UINT32_C(0x00418640));
                        return;
                    }
                    state->settings->mission_id = (int8_t)
                        state->index->records[next].link.mission_id;
                } else {
                    choice_pending = 1;
                }
            }
        } else if (state->settings->mission_id == 0x28) {
            operations->update_special_marker(operations->user_data);
            operations->save_profile(operations->user_data);
        }
    }

    for (int32_t value = 0; value < 3; ++value) {
        *state->city_end_values[value] = 0;
    }

    if (state->scenario_mode != 0) {
        *state->selection_flag = 0;
        *state->selection_byte = 0;
        *state->map_campaign_mode = 0;
        *state->map_mode = 3;
        operations->show_map_selection(operations->user_data);
        goto done;
    }
    if (next >= 0 && *state->game_is_replay != 3) {
        if (choice_pending != 0) {
            *state->selected_choice = 0;
            *state->current_window_id = 0x34;
        } else {
            operations->begin_briefing(operations->user_data);
        }
        goto done;
    }

    if (*state->game_is_replay == 1) {
        const int32_t chapter = (int32_t)(int8_t)state->settings->campaign + 1;
        *state->latest_period = (uint32_t)chapter;
        if (chapter == 5) {
            const char *const movie = state->settings->cpu < 233 ?
                "Binks\\low\\win_grand_small.bik" :
                "Binks\\high\\win_grand_big.bik";
            operations->play_video(operations->user_data, movie,
                (state->screen_width - 480) / 2,
                (state->screen_height - 286) / 2);
        } else if (chapter < 9) {
            *state->selected_period = (uint32_t)chapter;
            *state->campaign_menu_mode = 0;
            *state->current_window_id = 0x17;
            goto done;
        }
    }
    operations->show_main_menu(operations->user_data);

done:
    *state->simulation_flag_a = 0;
    *state->simulation_flag_b = 0;
    *state->redraw_requested = 1;
}
