#include "pharaoh/ui/victory_dialog.h"

enum {
    VICTORY_TEXT_GROUP = 62,
    VICTORY_PANEL_X = 48,
    VICTORY_PANEL_Y = 128,
    VICTORY_PANEL_WIDTH_BLOCKS = 34,
    VICTORY_PANEL_HEIGHT_BLOCKS = 15,
    VICTORY_PANEL_WIDTH = 544,
    VICTORY_LABEL = 6,
    VICTORY_TEXT_TITLE = 0,
    VICTORY_TEXT_CLICK_TO_PROCEED = 2,
    VICTORY_TEXT_PROCEED = 3,
    VICTORY_TEXT_CONTINUE_2_YEARS = 4,
    VICTORY_TEXT_CONTINUE_5_YEARS = 5,
    VICTORY_TEXT_NEW_GAME = 6,
    VICTORY_TEXT_REPLAY_MISSION = 37,
    VICTORY_TEXT_LOWER_DIFFICULTY = 40,
    MONUMENT_TEXT_GROUP = 198
};

static void set_dialog_geometry(PharaohVictoryDialogState *state)
{
    *state->panel_x = state->origin_x + VICTORY_PANEL_X;
    *state->panel_y = state->origin_y + VICTORY_PANEL_Y;
}

static void draw_button(const PharaohVictoryDialogState *state,
    const PharaohVictoryDialogOperations *operations, int32_t button,
    int32_t dx, int32_t dy, int32_t width_blocks, int32_t text)
{
    const int32_t x = *state->panel_x + dx;
    const int32_t y = *state->panel_y + dy;
    operations->draw_large_label(operations->user_data, VICTORY_LABEL,
        x, y, width_blocks, state->focused_button == button);
    operations->draw_language_centered(operations->user_data,
        VICTORY_TEXT_GROUP, text, x, y + 6, width_blocks * 16,
        state->black_font, 0);
}

void pharaoh_victory_dialog_draw_background(
    PharaohVictoryDialogState *state,
    const PharaohVictoryDialogOperations *operations)
{
    set_dialog_geometry(state);
    *state->view_x = (1024 - state->screen_width) / 2;
    *state->view_y = (768 - state->screen_height) / 2;
    *state->view_width = state->screen_width;
    *state->view_height = state->screen_height;
    operations->draw_panel(operations->user_data, *state->panel_x,
        *state->panel_y, VICTORY_PANEL_WIDTH_BLOCKS,
        VICTORY_PANEL_HEIGHT_BLOCKS);
    operations->draw_language_centered(operations->user_data,
        VICTORY_TEXT_GROUP, VICTORY_TEXT_TITLE, *state->panel_x,
        *state->panel_y + 16, VICTORY_PANEL_WIDTH, state->large_font, 0);
    operations->draw_language_centered(operations->user_data,
        VICTORY_TEXT_GROUP, VICTORY_TEXT_CLICK_TO_PROCEED,
        *state->panel_x, *state->panel_y + 47, VICTORY_PANEL_WIDTH,
        state->black_font, 0);
}

void pharaoh_victory_dialog_draw_foreground(
    PharaohVictoryDialogState *state,
    const PharaohVictoryDialogOperations *operations)
{
    set_dialog_geometry(state);
    if (state->victory_state == PHARAOH_VICTORY_STATE_WON) {
        draw_button(state, operations, 1, 32, 112, 30,
            VICTORY_TEXT_PROCEED);
        if (state->mission_id > 1 || state->scenario_mode != 0 ||
            state->campaign_mode != 0) {
            draw_button(state, operations, 2, 32, 144, 30,
                VICTORY_TEXT_CONTINUE_2_YEARS);
            draw_button(state, operations, 3, 32, 176, 30,
                VICTORY_TEXT_CONTINUE_5_YEARS);
        }
        return;
    }

    if (operations->defeat_options(operations->user_data) ==
        PHARAOH_DEFEAT_NEW_GAME) {
        draw_button(state, operations, 1, 64, 96, 26,
            VICTORY_TEXT_NEW_GAME);
    } else if (operations->defeat_options(operations->user_data) ==
        PHARAOH_DEFEAT_REPLAY) {
        draw_button(state, operations, 1, 64, 96, 9,
            VICTORY_TEXT_NEW_GAME);
        draw_button(state, operations, 2, 352, 96, 9,
            VICTORY_TEXT_REPLAY_MISSION);
    } else {
        draw_button(state, operations, 1, 32, 96, 9,
            VICTORY_TEXT_LOWER_DIFFICULTY);
        draw_button(state, operations, 2, 208, 96, 9,
            VICTORY_TEXT_NEW_GAME);
        draw_button(state, operations, 3, 384, 96, 9,
            VICTORY_TEXT_REPLAY_MISSION);
    }
}

int32_t pharaoh_victory_count_finished_monuments(
    const PharaohVictoryMonumentState *state,
    const PharaohVictoryMonumentOperations *operations)
{
    int32_t count = 0;
    int32_t line = 180;
    for (int32_t slot = 0; slot < 3; ++slot) {
        const int32_t building_id = state->monument_building_ids[slot];
        if (building_id <= 0) continue;

        const int32_t type = operations->building_type(
            operations->user_data, building_id);
        uint32_t progress;
        switch (type) {
        case 0xb7:
            progress = operations->pyramid_progress(
                operations->user_data, building_id);
            break;
        case 0xcf:
        case 0xd0:
            progress = operations->stored_progress(
                operations->user_data, building_id);
            break;
        case 0xd2:
            progress = (uint32_t)((state->city_progress_sixteenths * 100) /
                16);
            break;
        case 0xd5:
        case 0xde:
        case 0xe1:
        case 0xe3:
        case 0xe4:
        case 0xe5:
        case 0xe6:
        case 0xea:
        case 0xeb:
        case 0xec:
            progress = operations->monument_progress(
                operations->user_data, type, building_id);
            break;
        default:
            continue;
        }
        if (progress != 100) continue;

        line += 20;
        ++count;
        operations->draw_language(operations->user_data,
            MONUMENT_TEXT_GROUP, state->monument_text_ids[slot],
            state->panel_x + 275, state->panel_y + line,
            state->black_font, 0);
    }
    return count;
}
