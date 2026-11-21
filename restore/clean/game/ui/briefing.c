#include "pharaoh/ui/briefing.h"

enum {
    BRIEFING_WINDOW_CITY = 0x01,
    BRIEFING_WINDOW_MAIN = 0x35,
    BRIEFING_PANEL_INSET = 16,
    BRIEFING_PANEL_COLUMNS = 38,
    BRIEFING_PANEL_ROWS = 28,
    BRIEFING_TEXT_COLUMNS = 35,
    BRIEFING_TEXT_GROUP = 62,
    BRIEFING_EXIT_TEXT = 7,
    BRIEFING_GOAL_TITLE_TEXT = 10,
    BRIEFING_POPULATION_TEXT = 11,
    BRIEFING_GOAL_FIRST_TEXT = 12,
    BRIEFING_GOAL_COUNT = 4,
    BRIEFING_BACKGROUND_GROUP = 18,
    BRIEFING_TITLE_FONT = 640,
    BRIEFING_BODY_FONT = 635,
    BRIEFING_HEADING_FONT = 637,
    BRIEFING_GOAL_FONT = 638,
    BRIEFING_DIFFICULTY_PREFIX_GROUP = 44,
    BRIEFING_DIFFICULTY_PREFIX_TEXT = 216,
    BRIEFING_DIFFICULTY_GROUP = 153,
    BRIEFING_HOUSING_GROUP = 29,
    BRIEFING_HOUSING_FIRST_TEXT = 20,
    BRIEFING_BACK_GROUP = 13,
    BRIEFING_BACK_TEXT = 4,
    BRIEFING_FRAME_STYLE = 6,
    BRIEFING_FRAME_COLUMNS = 15,
    BRIEFING_TEXT_WIDTH = 35 * 16 - 16,
    BRIEFING_VIEWPORT_LINES = 14
};

static const PharaohBriefingPoint goal_positions[6] = {
    {32, 90}, {288, 90}, {32, 112},
    {288, 112}, {32, 134}, {288, 134}
};

static void draw_goal_frame(const PharaohBriefingOperations *operations,
    int32_t panel_x, int32_t panel_y, PharaohBriefingPoint point,
    int32_t width_blocks)
{
    operations->draw_label_frame(operations->user_data,
        BRIEFING_FRAME_STYLE, panel_x + point.x, panel_y + point.y,
        width_blocks, 0, 1);
}

static int32_t draw_goal_number(const PharaohBriefingOperations *operations,
    int32_t value, int32_t x, int32_t y)
{
    return operations->draw_number(operations->user_data, value, '@',
        x, y, BRIEFING_GOAL_FONT, 0);
}

void pharaoh_briefing_draw(PharaohBriefingState *state,
    const PharaohBriefingScenario *scenario,
    const PharaohBriefingMessage *message, int32_t campaign_has_choices,
    const PharaohBriefingOperations *operations)
{
    const int32_t panel_x = state->origin_x + BRIEFING_PANEL_INSET;
    const int32_t panel_y = state->origin_y + BRIEFING_PANEL_INSET;
    int32_t position;

    if (state->started == 0) {
        state->started = 1;
        operations->load_selected_mission(operations->user_data);
        if (state->current_window_id == BRIEFING_WINDOW_CITY) return;
    }
    if (state->redraw_requested == 1) {
        operations->draw_background(operations->user_data,
            BRIEFING_BACKGROUND_GROUP, 1, 0, 0);
    }

    operations->draw_panel(operations->user_data, panel_x, panel_y,
        BRIEFING_PANEL_COLUMNS, BRIEFING_PANEL_ROWS);
    operations->draw_text(operations->user_data, message->title,
        panel_x + 16, panel_y + 16, BRIEFING_TITLE_FONT, 0);
    operations->draw_text(operations->user_data, message->subtitle,
        panel_x + 16, panel_y + 46, BRIEFING_BODY_FONT, 0);
    (void)operations->draw_language(operations->user_data,
        BRIEFING_TEXT_GROUP, BRIEFING_EXIT_TEXT,
        panel_x + 476, panel_y + 417, BRIEFING_BODY_FONT, 0);
    if (state->current_window_id == BRIEFING_WINDOW_MAIN &&
        campaign_has_choices != 0) {
        (void)operations->draw_language(operations->user_data,
            BRIEFING_BACK_GROUP, BRIEFING_BACK_TEXT,
            panel_x + 50, panel_y + 419, BRIEFING_BODY_FONT, 0);
    }

    int32_t difficulty_x = operations->draw_language(operations->user_data,
        BRIEFING_DIFFICULTY_PREFIX_GROUP,
        BRIEFING_DIFFICULTY_PREFIX_TEXT, panel_x + 150, panel_y + 417,
        BRIEFING_BODY_FONT, 0);
    (void)operations->draw_language(operations->user_data,
        BRIEFING_DIFFICULTY_GROUP, state->difficulty + 1,
        difficulty_x, panel_y + 417, BRIEFING_BODY_FONT, 0);

    operations->draw_inner_panel(operations->user_data,
        panel_x + 16, panel_y + 64, BRIEFING_TEXT_COLUMNS + 1, 6);
    (void)operations->draw_language(operations->user_data,
        BRIEFING_TEXT_GROUP, BRIEFING_GOAL_TITLE_TEXT,
        panel_x + 32, panel_y + 72, BRIEFING_HEADING_FONT, 0);

    position = scenario->population_enabled != 0;
    if (scenario->population_enabled != 0) {
        const PharaohBriefingPoint point = goal_positions[0];
        draw_goal_frame(operations, panel_x, panel_y, point,
            BRIEFING_FRAME_COLUMNS);
        int32_t x = operations->draw_language(operations->user_data,
            BRIEFING_TEXT_GROUP, BRIEFING_POPULATION_TEXT,
            panel_x + point.x + 8, panel_y + point.y + 3,
            BRIEFING_GOAL_FONT, 0);
        (void)draw_goal_number(operations, scenario->population, x,
            panel_y + point.y + 3);
    }
    if (scenario->houses != 0 && position < 6) {
        const PharaohBriefingPoint point = goal_positions[position++];
        draw_goal_frame(operations, panel_x, panel_y, point,
            BRIEFING_FRAME_COLUMNS);
        int32_t x = draw_goal_number(operations, scenario->houses,
            panel_x + point.x + 8, panel_y + point.y + 3);
        (void)operations->draw_language(operations->user_data,
            BRIEFING_HOUSING_GROUP,
            scenario->houses_level + BRIEFING_HOUSING_FIRST_TEXT,
            x, panel_y + point.y + 3, BRIEFING_GOAL_FONT, 0);
    }
    for (int32_t goal = 0; goal < BRIEFING_GOAL_COUNT && position < 6;
         ++goal) {
        if (scenario->rating_enabled[goal] != 0) {
            const PharaohBriefingPoint point = goal_positions[position++];
            draw_goal_frame(operations, panel_x, panel_y, point,
                BRIEFING_FRAME_COLUMNS);
            int32_t x = operations->draw_language(operations->user_data,
                BRIEFING_TEXT_GROUP, BRIEFING_GOAL_FIRST_TEXT + goal,
                panel_x + point.x + 8, panel_y + point.y + 3,
                BRIEFING_GOAL_FONT, 0);
            (void)draw_goal_number(operations,
                scenario->rating_values[goal], x,
                panel_y + point.y + 3);
        }
    }

    const char *const hint = operations->tutorial_hint != 0 ?
        operations->tutorial_hint(operations->user_data) : 0;
    if (hint != 0 && *hint != 0) {
        const PharaohBriefingPoint point = goal_positions[4];
        draw_goal_frame(operations, panel_x, panel_y, point,
            BRIEFING_TEXT_COLUMNS - 1);
        operations->draw_text(operations->user_data, hint,
            panel_x + point.x + 8, panel_y + point.y + 3,
            BRIEFING_GOAL_FONT, 0);
    }

    operations->draw_inner_panel(operations->user_data,
        panel_x + 16, panel_y + 168, BRIEFING_TEXT_COLUMNS - 2, 15);
    if (state->line_count == 0) {
        state->viewport_lines = BRIEFING_VIEWPORT_LINES;
        state->scroll_up_enabled = 1;
        state->scroll_down_enabled = 1;
        state->line_count = operations->draw_rich_text(
            operations->user_data, message->body, panel_x + 32,
            panel_y + 184, BRIEFING_TEXT_WIDTH, 1, 0,
            state->scroll_position, state->viewport_lines);
        if (state->line_count > state->viewport_lines) {
            state->scroll_maximum =
                state->line_count - state->viewport_lines;
        } else {
            state->scroll_maximum = 0;
            state->scroll_up_enabled = 0;
            state->scroll_down_enabled = 0;
        }
    }
    if (state->line_count > state->viewport_lines) {
        operations->draw_inner_panel(operations->user_data,
            panel_x + 557, panel_y + 192, 2, 12);
    }
    (void)operations->draw_rich_text(operations->user_data,
        message->body, panel_x + 32, panel_y + 180,
        BRIEFING_TEXT_WIDTH, 0, 0, state->scroll_position,
        state->viewport_lines);
}
