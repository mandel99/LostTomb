#include "pharaoh/ui/mission_intermezzo.h"

enum {
    MISSION_CHOICE_SIZE = 44,
    DEFAULT_TRANSITION_TEXT_ID = 37,
    NO_MISSION = 0x7f
};

void pharaoh_mission_choice_handle_input(
    PharaohMissionChoiceInputState *state,
    const PharaohMissionChoiceInputOperations *operations)
{
    if (state->input_enabled == 0) return;

    const int32_t selected = state->settings->campaign_link;
    if (selected < 0 || selected >= state->index->count) {
        operations->selection_failure(operations->user_data,
            UINT32_C(0x00419090));
        return;
    }

    const PharaohCampaignLink *const link =
        &state->index->records[selected].link;
    for (int32_t choice = 0; choice < link->choice_count; ++choice) {
        const PharaohCampaignChoice *const item = &link->choices[choice];
        if (operations->mouse_inside(operations->user_data,
                item->x + state->origin_x, item->y + state->origin_y,
                MISSION_CHOICE_SIZE, MISSION_CHOICE_SIZE) != 0) {
            *state->selected_choice = (uint8_t)(choice + 1);
            operations->play_fanfare(operations->user_data,
                (choice & 1) != 0 ? "wavs\\Fanfare_Nu5.wav" :
                                    "wavs\\Fanfare_Nu1.wav",
                1, 0);
            *state->redraw_requested = 1;
            return;
        }
    }
}

void pharaoh_intermezzo_prepare(PharaohIntermezzoState *state,
    const PharaohIntermezzoOperations *operations)
{
    *state->view_width = state->screen_width;
    *state->view_x = (1024 - state->screen_width) / 2;
    *state->view_height = state->screen_height;
    *state->view_y = (768 - state->screen_height) / 2;

    if (state->mode == 1) {
        operations->draw_sprite(operations->user_data,
            state->background_image, 0, 0);
        return;
    }
    if (state->mode != 0 && state->mode != 2) return;

    if (state->mode == 2) {
        *state->music_flag_a = 1;
        *state->music_flag_b = 1;
    }
    if (state->scenario_mode != 0) {
        operations->draw_sprite(operations->user_data,
            state->background_image + (state->mode == 0), 0, 0);
        *state->redraw_requested = 1;
        return;
    }
    if (state->mission_id != NO_MISSION) {
        operations->draw_sprite(operations->user_data,
            state->background_image + (state->mode == 0), 0, 0);
        if (state->mode == 0) {
            *state->music_flag_a = 1;
            *state->music_flag_b = 1;
        }
        operations->play_mission_track(operations->user_data,
            state->mode == 2, state->classical_names, state->mission_id);
    }
    *state->redraw_requested = 1;
}

int32_t pharaoh_mission_transition_text_id(
    const PharaohCampaignIndex *index, const PharaohSettingsFile *settings,
    int32_t scenario_mode)
{
    if (scenario_mode == 0) {
        const int32_t selected = settings->campaign_link;
        if (selected >= 0 && selected < index->count) {
            return index->records[selected].link.transition_text_id;
        }
    }
    return DEFAULT_TRANSITION_TEXT_ID;
}

static const PharaohCampaignLink *selected_link(
    const PharaohMissionChoiceRenderState *state)
{
    const int32_t selected = state->settings->campaign_link;
    if (selected < 0 || selected >= state->index->count) return 0;
    return &state->index->records[selected].link;
}

void pharaoh_mission_choice_draw_markers(
    const PharaohMissionChoiceRenderState *state,
    const PharaohMissionChoiceRenderOperations *operations)
{
    const PharaohCampaignLink *const link = selected_link(state);
    if (link == 0) return;
    for (int32_t choice = 0; choice < link->choice_count; ++choice) {
        const PharaohCampaignChoice *const item = &link->choices[choice];
        const int32_t x = item->x + state->origin_x;
        const int32_t y = item->y + state->origin_y;
        int32_t marker;
        if (operations->mouse_inside(operations->user_data, x, y,
                MISSION_CHOICE_SIZE, MISSION_CHOICE_SIZE) != 0) {
            marker = 1;
        } else {
            marker = (int32_t)state->selected_choice - 1 == choice ? 2 : 0;
        }
        operations->draw_sprite(operations->user_data,
            state->marker_image_base + marker, x, y);
    }
}

void pharaoh_mission_choice_draw(
    PharaohMissionChoiceRenderState *state,
    const PharaohMissionChoiceRenderOperations *operations)
{
    const PharaohCampaignLink *const link = selected_link(state);
    if (link == 0) return;

    *state->panel_x = state->origin_x;
    *state->panel_y = state->origin_y;
    *state->view_x = (1024 - state->screen_width) / 2;
    *state->view_y = (768 - state->screen_height) / 2;
    *state->view_width = state->screen_width;
    *state->view_height = state->screen_height;
    operations->draw_sprite(operations->user_data,
        state->choice_background_image, 0, 0);
    operations->draw_sprite(operations->user_data,
        state->choice_link_image_base + link->background_id,
        *state->panel_x, *state->panel_y);
    operations->draw_language(operations->user_data, 144, link->text_id,
        *state->panel_x + 20, *state->panel_y + 410,
        state->large_font, 0);
    int32_t text_id = 0;
    if (state->selected_choice != 0) {
        text_id = link->choices[state->selected_choice - 1].text_id;
    }
    operations->draw_language_multiline(operations->user_data, 144, text_id,
        *state->panel_x + 20, *state->panel_y + 440, 560,
        state->black_font, 0);
}
