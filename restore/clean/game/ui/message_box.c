#include "pharaoh/ui/message_box.h"

enum { MESSAGE_BOX_BUTTON_TEXT_GROUP = 18 };

static int32_t configure_buttons(PharaohMessageBoxState *state, uint32_t type)
{
    static const int32_t labels[6][3] = {
        {3, 0, 0}, {3, 2, 0}, {10, 9, 11},
        {1, 0, 2}, {1, 0, 0}, {9, 2, 0}
    };
    static const int32_t results[6][3] = {
        {1, 0, 0}, {1, 2, 0}, {3, 4, 5},
        {6, 7, 2}, {6, 7, 0}, {4, 2, 0}
    };
    static const int32_t counts[6] = {1, 2, 3, 3, 2, 2};
    uint32_t style = type & 15u;
    int32_t index;

    if (style > 5u) style = 0;
    state->button_count = counts[style];
    for (index = 0; index < state->button_count; ++index) {
        state->buttons[index].label = labels[style][index];
        state->buttons[index].result = results[style][index];
    }
    return state->button_count;
}

void pharaoh_message_box_select(PharaohMessageBoxState *state,
    int32_t selected_index)
{
    state->activated = 1;
    state->selected_index = selected_index;
}

int32_t pharaoh_message_box_draw(PharaohMessageBoxState *state,
    const PharaohMessageBoxOperations *operations,
    const char *title, const char *body, uint32_t type)
{
    const int32_t middle_x = state->screen_width / 2;
    const int32_t middle_y = state->screen_height / 2;
    const int32_t body_width = operations->measure_text(
        operations->user_data, body, state->body_font);
    const int32_t title_width = operations->measure_text(
        operations->user_data, title, state->title_font);
    const int32_t text_width = title_width > body_width ?
        title_width : body_width;
    const int32_t span = text_width + 20;
    const int32_t left = middle_x - span / 2;
    const int32_t top = middle_y - 50;
    const int32_t panel_units = (span + 15) / 16;
    const int32_t count = configure_buttons(state, type);
    const int32_t button_width = span / 3;
    const int32_t button_y = middle_y + 20;
    const int32_t button_height = 25;
    int32_t button_x = middle_x - (button_width / 2) * count;
    int32_t index;

    operations->draw_panel(operations->user_data, left, top,
        panel_units + 1, 7);
    operations->draw_inner_panel(operations->user_data, left + 8, top + 5,
        panel_units, 2);
    operations->draw_centered_text(operations->user_data, title,
        left + 16, top + 10, span, state->title_draw_font);
    operations->draw_centered_text(operations->user_data, body,
        left + 5, top + 48, span - 10, state->body_font);

    for (index = 0; index < count; ++index) {
        PharaohMessageBoxButton *const button = &state->buttons[index];
        const uint32_t color = state->focused_button == index + 1 ?
            UINT32_C(0xf800) : 0;
        operations->draw_button_label(operations->user_data,
            MESSAGE_BOX_BUTTON_TEXT_GROUP, button->label,
            button_x, button_y + 8, button_width, state->body_font);
        operations->draw_button(operations->user_data, button_x, button_y,
            button_width, button_height, color);
        button->left = (int16_t)button_x;
        button->top = (int16_t)button_y;
        button->right = (int16_t)(button_x + button_width);
        button->bottom = (int16_t)(button_y + button_height);
        button_x += button_width;
    }
    if (state->activated == 0) return -1;
    return state->buttons[state->selected_index].result;
}

int32_t pharaoh_message_box_show(PharaohMessageBoxModalState *state,
    const PharaohMessageBoxModalOperations *operations,
    const char *title, const char *body, uint32_t type)
{
    const uint8_t previous_window = *state->current_window_id;
    int32_t selected = 0;

    *state->current_window_id = UINT8_C(0x54);
    state->box->activated = 0;
    for (;;) {
        if (operations->has_message(operations->user_data) != 0) {
            if (operations->get_translate_dispatch(
                    operations->user_data) == 0) {
                *state->redraw_request = 1;
                *state->current_window_id = previous_window;
                return selected;
            }
        } else if (*state->display_unavailable == 0 &&
                *state->display_ready != 0) {
            selected = operations->run_frame(operations->user_data,
                title, body, type);
            if (selected != -1) break;
        } else {
            operations->wait_message(operations->user_data);
        }
        if (*state->post_close_requested == 1) {
            operations->post_close(operations->user_data);
            *state->post_close_requested = 0;
        }
    }
    *state->current_window_id = previous_window;
    *state->redraw_request = 1;
    return selected;
}
