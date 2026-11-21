#include "pharaoh/ui/victory_dialog.h"

#include <string.h>

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

enum {
    VICTORY_RESULT_TEXT_GROUP = 148,
    TRANSITION_TEXT_GROUP = 147,
    DIFFICULTY_TEXT_GROUP = 153,
    WINDOW_HINT_TEXT_GROUP = 13,
    WINDOW_HINT_RIGHT_CLICK = 1,
    RESULT_TEXT_CULTURE = 0,
    RESULT_TEXT_PROSPERITY = 1,
    RESULT_TEXT_KINGDOM = 3,
    RESULT_TEXT_POPULATION = 4,
    RESULT_TEXT_TREASURY = 5,
    RESULT_TEXT_MONUMENTS = 6,
    RESULT_TEXT_MONTHS = 7,
    RESULT_TEXT_DIFFICULTY = 8,
    RESULT_TEXT_SCORE = 9,
    RESULT_TEXT_HIGH_SCORE_NEW = 10,
    RESULT_TEXT_HIGH_SCORE_OWN_KEPT = 11,
    RESULT_TEXT_HIGH_SCORE_OWN_BEATEN = 12,
    RESULT_TEXT_HIGH_SCORE_OTHER_KEPT = 13,
    RESULT_TEXT_HIGH_SCORE_OTHER_BEATEN = 14,
    RESULT_TEXT_MONTHS_POSTFIX = 15,
    RESULT_TEXT_CHEATED = 16,
    DEFEAT_TEXT_TITLE = 1,
    DEFEAT_TEXT_BODY = 16,
    DEFEAT_TEXT_TITLE_LOWER_DIFFICULTY = 38,
    DEFEAT_TEXT_BODY_LOWER_DIFFICULTY = 39
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

static void draw_rating(PharaohVictoryScreenState *state,
    const PharaohVictoryScreenOperations *operations, int32_t text,
    int32_t dx, int32_t dy)
{
    *state->text_width = 0;
    operations->draw_language(operations->user_data,
        VICTORY_RESULT_TEXT_GROUP, text, *state->panel_x + dx,
        *state->panel_y + dy, state->black_font, 0);
    const int32_t value = operations->rating_value(
        operations->user_data, text);
    operations->draw_number(operations->user_data, value,
        *state->panel_x + dx + *state->text_width,
        *state->panel_y + dy, state->black_font, 0);
}

static void draw_expanded_high_score(PharaohVictoryScreenState *state,
    const PharaohVictoryScreenOperations *operations, int32_t text,
    const char *name)
{
    char saved_name[32];
    char expanded[200];
    const char *const message = operations->language_string(
        operations->user_data, VICTORY_RESULT_TEXT_GROUP, text);
    memcpy(saved_name, state->player_name, strlen(state->player_name) + 1);
    memcpy(state->player_name, name, strlen(name) + 1);
    operations->expand_event_text(operations->user_data, message,
        expanded, 200);
    memcpy(state->player_name, saved_name, strlen(saved_name) + 1);
    operations->draw_text_multiline(operations->user_data, expanded,
        *state->panel_x + 40, *state->panel_y + 340, 544,
        state->black_font, 0);
}

static void draw_high_score_result(PharaohVictoryScreenState *state,
    const PharaohVictoryScreenOperations *operations)
{
    int32_t text = -1;
    switch (state->high_score_result) {
    case PHARAOH_HIGH_SCORE_CHEATED:
        text = RESULT_TEXT_CHEATED;
        break;
    case PHARAOH_HIGH_SCORE_NEW:
        text = RESULT_TEXT_HIGH_SCORE_NEW;
        break;
    case PHARAOH_HIGH_SCORE_OWN_KEPT:
        text = RESULT_TEXT_HIGH_SCORE_OWN_KEPT;
        break;
    case PHARAOH_HIGH_SCORE_OWN_BEATEN:
        text = RESULT_TEXT_HIGH_SCORE_OWN_BEATEN;
        break;
    case PHARAOH_HIGH_SCORE_OTHER_BEATEN:
        draw_expanded_high_score(state, operations,
            RESULT_TEXT_HIGH_SCORE_OTHER_BEATEN,
            state->previous_high_score_name);
        return;
    case PHARAOH_HIGH_SCORE_OTHER_KEPT:
        draw_expanded_high_score(state, operations,
            RESULT_TEXT_HIGH_SCORE_OTHER_KEPT,
            state->mission_high_score_name);
        return;
    default:
        return;
    }
    operations->draw_language_multiline(operations->user_data,
        VICTORY_RESULT_TEXT_GROUP, text, *state->panel_x + 40,
        *state->panel_y + 340, 544, state->black_font, 0);
}

static void draw_defeat(PharaohVictoryScreenState *state,
    const PharaohVictoryScreenOperations *operations)
{
    int32_t title = DEFEAT_TEXT_TITLE;
    int32_t body = DEFEAT_TEXT_BODY;
    if (operations->defeat_options(operations->user_data) >=
        PHARAOH_DEFEAT_LOWER_DIFFICULTY) {
        title = DEFEAT_TEXT_TITLE_LOWER_DIFFICULTY;
        body = DEFEAT_TEXT_BODY_LOWER_DIFFICULTY;
    }
    operations->draw_panel(operations->user_data, *state->panel_x,
        *state->panel_y - 7, 34, 16);
    operations->draw_language_centered(operations->user_data,
        VICTORY_TEXT_GROUP, title, *state->panel_x, *state->panel_y + 16,
        544, state->large_font, 0);
    *state->text_width = 0;
    operations->draw_language_multiline(operations->user_data,
        VICTORY_TEXT_GROUP, body, *state->panel_x + 25,
        *state->panel_y + 48, 512, state->black_font, 0);
}

void pharaoh_victory_screen_draw(PharaohVictoryScreenState *state,
    const PharaohVictoryScreenOperations *operations)
{
    *state->panel_x = state->origin_x + 48;
    *state->panel_y = state->origin_y + 16;
    *state->view_x = (1024 - state->screen_width) / 2;
    *state->view_y = (768 - state->screen_height) / 2;
    *state->view_width = state->screen_width;
    *state->view_height = state->screen_height;
    if (state->victory_state != PHARAOH_VICTORY_STATE_WON) {
        draw_defeat(state, operations);
        return;
    }

    const uint32_t delta = (uint32_t)state->scenario_start_year -
        (uint32_t)state->current_year;
    const uint32_t sign = (uint32_t)((int32_t)delta >> 31);
    const uint32_t years = (delta ^ sign) - sign;
    const int32_t months = (int32_t)((uint32_t)state->current_month +
        years * 12u);
    const int32_t score = operations->victory_score(
        operations->user_data, months);
    operations->draw_panel(operations->user_data, *state->panel_x,
        *state->panel_y, 34, 27);
    operations->draw_language_centered(operations->user_data,
        VICTORY_TEXT_GROUP, VICTORY_TEXT_TITLE, *state->panel_x,
        *state->panel_y + 16, 544, state->large_font, 0);
    operations->draw_inner_panel(operations->user_data,
        *state->panel_x + 16, *state->panel_y + 56, 32, 7);
    operations->draw_language_multiline(operations->user_data,
        TRANSITION_TEXT_GROUP,
        operations->transition_text_id(operations->user_data),
        *state->panel_x + 32, *state->panel_y + 64, 496,
        state->white_font, 0);

    draw_rating(state, operations, RESULT_TEXT_CULTURE, 40, 180);
    draw_rating(state, operations, RESULT_TEXT_PROSPERITY, 40, 200);
    draw_rating(state, operations, RESULT_TEXT_KINGDOM, 40, 220);
    draw_rating(state, operations, RESULT_TEXT_TREASURY, 40, 240);
    draw_rating(state, operations, RESULT_TEXT_POPULATION, 40, 260);
    if (operations->count_finished_monuments(operations->user_data) != 0) {
        operations->draw_language(operations->user_data,
            VICTORY_RESULT_TEXT_GROUP, RESULT_TEXT_MONUMENTS,
            *state->panel_x + 275, *state->panel_y + 180,
            state->black_font, 0);
    }

    *state->text_width = 0;
    operations->draw_language(operations->user_data,
        VICTORY_RESULT_TEXT_GROUP, RESULT_TEXT_MONTHS,
        *state->panel_x + 120, *state->panel_y + 280,
        state->black_font, 0);
    operations->draw_number(operations->user_data, months,
        *state->panel_x + 120 + *state->text_width,
        *state->panel_y + 280, state->black_font, 0);
    operations->draw_language(operations->user_data,
        VICTORY_RESULT_TEXT_GROUP, RESULT_TEXT_MONTHS_POSTFIX,
        *state->panel_x + 120 + *state->text_width,
        *state->panel_y + 280, state->black_font, 0);

    *state->text_width = 0;
    operations->draw_language(operations->user_data,
        VICTORY_RESULT_TEXT_GROUP, RESULT_TEXT_DIFFICULTY,
        *state->panel_x + 160, *state->panel_y + 300,
        state->black_font, 0);
    operations->draw_language(operations->user_data,
        DIFFICULTY_TEXT_GROUP,
        operations->difficulty(operations->user_data) + 1,
        *state->panel_x + 160 + *state->text_width,
        *state->panel_y + 300, state->black_font, 0);

    *state->text_width = 0;
    operations->draw_language(operations->user_data,
        VICTORY_RESULT_TEXT_GROUP, RESULT_TEXT_SCORE,
        *state->panel_x + 200, *state->panel_y + 320,
        state->black_font, 0);
    operations->draw_number(operations->user_data, score,
        *state->panel_x + 200 + *state->text_width,
        *state->panel_y + 320, state->black_font, 0);
    draw_high_score_result(state, operations);
    operations->draw_language_centered(operations->user_data,
        WINDOW_HINT_TEXT_GROUP, WINDOW_HINT_RIGHT_CLICK,
        *state->panel_x + 16, *state->panel_y + 396, 512,
        state->black_font, 0);
}
