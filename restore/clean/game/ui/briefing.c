#include "pharaoh/ui/briefing.h"
#include "pharaoh/runtime/mouse_update.h"

/* Complete one-callee C translation of Pharaoh.exe 0x004C92A0. */
int32_t pharaoh_briefing_trade_tooltip_resource(
    const PharaohBriefingTradeGoods *goods, int32_t origin_x,
    int32_t origin_y, int32_t mouse_x, int32_t mouse_y)
{
    int32_t column;
    for (column = 0; column < 17; ++column) {
        const int32_t first = goods->first_row[column];
        const int32_t second = goods->second_row[column];
        const int32_t x = first * 35 + 21 + origin_x;
        if (pharaoh_mouse_inside_rect(mouse_x, mouse_y,
            x, origin_y + 50, 24, 24)) {
            return first;
        }
        if (second != 27 && second >= 0 && second < 36 &&
            pharaoh_mouse_inside_rect(mouse_x, mouse_y,
                x, origin_y + 134, 24, 24)) {
            return second;
        }
    }
    return 0;
}

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

/* Complete C translation of Pharaoh.exe 0x00417E70. */
void pharaoh_mission_briefing_set_started(PharaohBriefingState *state)
{
    state->started = 1;
}

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
    const PharaohBriefingOperations *operations)
{
    int32_t position;

    if (state->started == 0) {
        state->started = 1;
        operations->load_selected_mission(operations->user_data);
        if (state->current_window_id == BRIEFING_WINDOW_CITY) return;
    }
    state->message_id = operations->get_message_id(operations->user_data);
    const PharaohBriefingMessage *const message = operations->message_at(
        operations->user_data, state->message_id);
    const int32_t panel_x = state->origin_x + BRIEFING_PANEL_INSET;
    const int32_t panel_y = state->origin_y + BRIEFING_PANEL_INSET;
    state->panel_x = panel_x;
    state->panel_y = panel_y;
    state->view_width = state->window_width;
    state->view_height = state->window_height;
    state->view_x = (1024 - state->window_width) / 2;
    state->view_y = (768 - state->window_height) / 2;
    state->text_width_blocks = BRIEFING_TEXT_COLUMNS;
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
        operations->campaign_has_choices(operations->user_data) != 0) {
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

    state->label_progress = 100;
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
    if (hint != 0) {
        const PharaohBriefingPoint point = goal_positions[4];
        draw_goal_frame(operations, panel_x, panel_y, point,
            BRIEFING_TEXT_COLUMNS - 1);
        operations->draw_text(operations->user_data, hint,
            panel_x + point.x + 8, panel_y + point.y + 3,
            BRIEFING_GOAL_FONT, 0);
    }

    operations->draw_inner_panel(operations->user_data,
        panel_x + 16, panel_y + 168, BRIEFING_TEXT_COLUMNS - 2, 15);
    state->normal_font = BRIEFING_HEADING_FONT;
    state->link_font = BRIEFING_GOAL_FONT;
    state->text_x = panel_x - 6;
    state->text_y = panel_y + 170;
    if (state->line_count == 0) {
        state->text_height_blocks = 15;
        state->viewport_lines = BRIEFING_VIEWPORT_LINES;
        state->scroll_up_enabled = 1;
        state->scroll_down_enabled = 1;
        state->line_count = operations->draw_rich_text(
            operations->user_data, message->body, panel_x + 32,
            panel_y + 184, BRIEFING_TEXT_WIDTH, 1, 0,
            state->scroll_position, state->viewport_lines, state);
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
    state->clip_x_min = panel_x + 19;
    state->clip_x_max = panel_x + BRIEFING_TEXT_COLUMNS * 16 + 19;
    state->clip_y_min = panel_y + 171;
    state->clip_y_max = panel_y + 405;
    (void)operations->draw_rich_text(operations->user_data,
        message->body, panel_x + 32, panel_y + 180,
        BRIEFING_TEXT_WIDTH, 0, 0, state->scroll_position,
        state->viewport_lines, state);
    state->clip_x_min = 0;
    state->clip_x_max = state->window_width;
    state->clip_y_min = 0;
    state->clip_y_max = state->window_height;
}

void pharaoh_briefing_window_input(uint8_t window_id, int32_t panel_x,
    int32_t panel_y, const PharaohBriefingWindowInputOperations *operations)
{
    if (operations->handle_image_buttons(operations->user_data,
            panel_x + 564, panel_y + 410,
            PHARAOH_BRIEFING_ACCEPT_BUTTON, 1) != 0) return;
    if (operations->handle_image_buttons(operations->user_data,
            panel_x + 110, panel_y + 416,
            PHARAOH_BRIEFING_DIFFICULTY_BUTTONS, 2) != 0) return;
    if (window_id == BRIEFING_WINDOW_MAIN &&
        operations->campaign_has_choices(operations->user_data) != 0 &&
        operations->handle_image_buttons(operations->user_data,
            panel_x + 10, panel_y + 412,
            PHARAOH_BRIEFING_BACK_BUTTON, 1) != 0) return;
    operations->handle_scroll(operations->user_data);
}

void pharaoh_briefing_handle_input(int32_t origin_x, int32_t origin_y,
    const PharaohBriefingInputOperations *operations)
{
    const int32_t x = origin_x + 560;
    const int32_t up_y = origin_y + 168;
    const int32_t down_y = origin_y + 382;

    operations->set_scroll_button_tooltip(operations->user_data, 0,
        x, up_y, 12);
    if (operations->handle_scroll_button(operations->user_data, 0,
            x, up_y) != 0) return;
    operations->set_scroll_button_tooltip(operations->user_data, 1,
        x, down_y, 13);
    if (operations->handle_scroll_button(operations->user_data, 1,
            x, down_y) == 0) {
        operations->handle_scrollbar(operations->user_data);
    }
}
