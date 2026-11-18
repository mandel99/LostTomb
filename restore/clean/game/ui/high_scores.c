#include "pharaoh/ui/high_scores.h"

#include <stddef.h>

enum {
    HIGH_SCORE_GROUP = 296,
    DIFFICULTY_GROUP = 153,
    BOTTOM_GROUP = 13,
    COLOR_DEFAULT = 0,
    SCROLL_TRACK_LENGTH = 257,
    SCROLL_BUTTON_X = 576,
    SCROLL_UP_Y = 96,
    SCROLL_DOWN_Y = 424,
    SCROLL_BUTTON_SIZE = 34
};

/* Native helpers 0x004CDE60/0x004CDE40 only receive bounded nonnegative
   high-score values here, so their signed IDIV paths reduce to these forms. */
static int32_t scale_percent(int32_t value, int32_t denominator)
{
    return denominator == 0 ? 0 : value * 100 / denominator;
}

static int32_t adjust_percent(int32_t value, int32_t percentage)
{
    return value * percentage / 100;
}

_Static_assert(sizeof(PharaohHighScoreRecord) == 0x4c,
    "native high-score record size");
_Static_assert(sizeof(((PharaohHighScoresState *)0)->records) == 0x1db0,
    "native high-score table size");
_Static_assert(offsetof(PharaohCampaign, mission_names) == 0x1904 &&
    offsetof(PharaohCampaign, mission_name_count) == 0x2c8c,
    "native high-score mission catalog offsets");
_Static_assert(0x006ff60c + offsetof(PharaohCampaign, mission_name_count) -
    offsetof(PharaohCampaign, mission_names) == 0x00700994,
    "native high-score mission count address");

void pharaoh_high_scores_bind_campaign(PharaohHighScoresState *state,
    const PharaohCampaign *campaign)
{
    state->mission_names = campaign->mission_names;
    state->record_count = campaign->mission_name_count;
}

static int32_t clamped_record_count(const PharaohHighScoresState *state)
{
    if (state->record_count < 0) return 0;
    if (state->record_count > PHARAOH_HIGH_SCORE_CAPACITY) {
        return PHARAOH_HIGH_SCORE_CAPACITY;
    }
    return state->record_count;
}

static int32_t contains(int32_t x, int32_t y, int32_t left, int32_t top,
    int32_t width, int32_t height)
{
    return left <= x && x < left + width && top <= y && y < top + height;
}

int32_t pharaoh_high_scores_read(PharaohHighScoresState *state,
    const PharaohHighScoresOperations *operations, const char *path)
{
    return operations->read_records(operations->user_data, path,
        state->records, sizeof(state->records)) != 0;
}

int32_t pharaoh_high_scores_load(PharaohHighScoresState *state,
    const PharaohHighScoresOperations *operations, const char *path)
{
    int32_t index;

    state->valid_count = 0;
    if (pharaoh_high_scores_read(state, operations, path) == 0) return 0;
    for (index = 0; index < clamped_record_count(state); ++index) {
        if (state->records[index].valid != 0) ++state->valid_count;
    }
    return 1;
}

void pharaoh_high_scores_scroll_button(PharaohHighScoresState *state,
    int32_t action)
{
    if (state->valid_count <= PHARAOH_HIGH_SCORE_VISIBLE_ROWS) return;
    if (action == 0) {
        if (state->first_visible <= 0) return;
        --state->first_visible;
        *state->redraw_request = 2;
        return;
    }
    if (action != 1) return;
    if (state->first_visible + PHARAOH_HIGH_SCORE_VISIBLE_ROWS >=
            state->valid_count) {
        return;
    }
    ++state->first_visible;
    *state->redraw_request = 2;
}

void pharaoh_high_scores_draw_scroll_thumb(
    const PharaohHighScoresState *state,
    const PharaohHighScoresOperations *operations)
{
    int32_t percent;
    int32_t y;

    if (state->valid_count <= PHARAOH_HIGH_SCORE_VISIBLE_ROWS) return;
    if (state->first_visible <= 0) {
        percent = 0;
    } else if (state->first_visible + PHARAOH_HIGH_SCORE_VISIBLE_ROWS <
            state->valid_count) {
        percent = scale_percent(state->first_visible,
            state->valid_count - PHARAOH_HIGH_SCORE_VISIBLE_ROWS);
    } else {
        percent = 100;
    }
    y = adjust_percent(SCROLL_TRACK_LENGTH, percent);
    operations->draw_sprite(operations->user_data,
        state->panel_image_base + 39, state->origin_x + 584,
        state->origin_y + 130 + y);
}

void pharaoh_high_scores_draw_rows(const PharaohHighScoresState *state,
    const PharaohHighScoresOperations *operations)
{
    int32_t row;
    int32_t pixels = 0;
    const int32_t count = clamped_record_count(state);

    if (state->valid_count == 0) {
        operations->draw_language(operations->user_data,
            HIGH_SCORE_GROUP, 6, state->origin_x + 48,
            state->origin_y + 104, state->empty_font, COLOR_DEFAULT);
        return;
    }
    for (row = 0; row < count; ++row) {
        const int32_t index = state->first_visible + row;
        int32_t family_x;
        if (index < 0 || index >= count) break;
        if (state->records[index].valid != 0 &&
            state->mission_names != NULL) {
            operations->draw_text(operations->user_data,
                state->mission_names[index], state->origin_x + 48,
                state->origin_y + 104 + pixels, state->small_font,
                COLOR_DEFAULT);
            family_x = operations->draw_language(operations->user_data,
                HIGH_SCORE_GROUP, 5, state->origin_x + 224,
                state->origin_y + 104 + pixels, state->small_font,
                COLOR_DEFAULT);
            operations->draw_text(operations->user_data,
                state->records[index].family, family_x,
                state->origin_y + 104 + pixels, state->small_font,
                COLOR_DEFAULT);
            operations->draw_language(operations->user_data,
                DIFFICULTY_GROUP, state->records[index].difficulty + 1,
                state->origin_x + 368, state->origin_y + 104 + pixels,
                state->small_font, COLOR_DEFAULT);
            operations->draw_number(operations->user_data,
                state->records[index].score, 64, state->origin_x + 512,
                state->origin_y + 104 + pixels, state->small_font,
                COLOR_DEFAULT);
            pixels += 16;
        }
        if (pixels >= 320) return;
    }
}

void pharaoh_high_scores_draw_background(PharaohHighScoresState *state,
    const PharaohHighScoresOperations *operations)
{
    static const int32_t column_x[4] = {80, 224, 368, 512};
    int32_t column;

    operations->draw_panel(operations->user_data, state->origin_x,
        state->origin_y, 40, 30);
    operations->draw_inner_panel(operations->user_data,
        state->origin_x + 32, state->origin_y + 96, 34, 22);
    operations->draw_centered_language(operations->user_data,
        HIGH_SCORE_GROUP, 0, state->origin_x + 20, state->origin_y + 32,
        608, state->title_font, COLOR_DEFAULT);
    operations->draw_inner_panel(operations->user_data,
        state->origin_x + 579, state->origin_y + 123, 2, 19);
    for (column = 0; column < 4; ++column) {
        operations->draw_language(operations->user_data,
            HIGH_SCORE_GROUP, column + 1, state->origin_x + column_x[column],
            state->origin_y + 80, state->normal_font, COLOR_DEFAULT);
    }
    pharaoh_high_scores_draw_rows(state, operations);
    pharaoh_high_scores_draw_scroll_thumb(state, operations);
    operations->draw_centered_language(operations->user_data,
        BOTTOM_GROUP, 1, state->origin_x + 20, state->origin_y + 452,
        608, state->normal_font, COLOR_DEFAULT);
    *state->redraw_request = 1;
}

int32_t pharaoh_high_scores_drag_scrollbar(PharaohHighScoresState *state,
    const PharaohHighScoresInput *input)
{
    int32_t y;
    int32_t percent;

    if (state->valid_count <= PHARAOH_HIGH_SCORE_VISIBLE_ROWS ||
        input->left_down == 0) {
        return 0;
    }
    if (input->cursor_x < state->origin_x + 568 ||
        input->cursor_x > state->origin_x + 600 ||
        input->cursor_y < state->origin_y + 128 ||
        input->cursor_y > state->origin_y + 401) {
        return 0;
    }
    y = input->cursor_y - state->origin_y - 128;
    if (y > SCROLL_TRACK_LENGTH) y = SCROLL_TRACK_LENGTH;
    percent = scale_percent(y, SCROLL_TRACK_LENGTH);
    state->first_visible = adjust_percent(
        state->valid_count - PHARAOH_HIGH_SCORE_VISIBLE_ROWS,
        percent);
    *state->redraw_request = 2;
    return 1;
}

int32_t pharaoh_high_scores_handle_input(PharaohHighScoresState *state,
    const PharaohHighScoresInput *input)
{
    if (input->right_released != 0) {
        *state->current_window_id = 0;
        *state->redraw_request = 1;
        return 1;
    }
    if (pharaoh_high_scores_drag_scrollbar(state, input) != 0) return 1;
    if (input->left_pressed == 0) return 0;
    if (contains(input->cursor_x, input->cursor_y,
            state->origin_x + SCROLL_BUTTON_X,
            state->origin_y + SCROLL_UP_Y,
            SCROLL_BUTTON_SIZE, SCROLL_BUTTON_SIZE)) {
        pharaoh_high_scores_scroll_button(state, 0);
        return 1;
    }
    if (contains(input->cursor_x, input->cursor_y,
            state->origin_x + SCROLL_BUTTON_X,
            state->origin_y + SCROLL_DOWN_Y,
            SCROLL_BUTTON_SIZE, SCROLL_BUTTON_SIZE)) {
        pharaoh_high_scores_scroll_button(state, 1);
        return 1;
    }
    return 0;
}
