#include "pharaoh/ui/message_actions.h"
#include "pharaoh/core/memory_fill.h"
#include "pharaoh/ui/rich_text.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohMenuMessageRecord) == 48,
    "native menu message record size");
_Static_assert(offsetof(PharaohMenuMessageRecord, message) == 10 &&
    offsetof(PharaohMenuMessageRecord, read) == 14 &&
    offsetof(PharaohMenuMessageRecord, template_id) == 16,
    "native menu message record fields");
_Static_assert(sizeof(PharaohMessageInputRow) == 20,
    "native message input row size");

void pharaoh_message_dialog_show(PharaohMessageDialogOpenState *state,
    int32_t message_id, uint32_t parameter, uint8_t has_phrases)
{
    *state->has_phrases = has_phrases;
    if (has_phrases == 0) {
        *state->phrase_count = 0;
    } else {
        *state->phrase_count = *state->current_message_phrase_count;
    }

    pharaoh_memory_fill_bytes(state->dialog_words_a, 200u * 4u, 0);
    pharaoh_memory_fill_bytes(state->dialog_words_b, 200u * 4u, 0);
    *state->message_id = message_id;
    *state->message_parameter = parameter;
    *state->dialog_state_aae0 = 0;
    *state->dialog_state_b130 = 0;
    *state->dialog_state_b138 = 0;
    *state->dialog_visible = *state->message_record_value != 0u;
    if (*state->event54_popup != 1u) *state->dialog_visible = 0;
    *state->dialog_state_b12c = 0;

    pharaoh_rich_text_reset_links(state->rich_text);
    *state->redraw_requested = 1;
    *state->previous_window_id = *state->current_window_id;
    *state->current_window_id = 5;
}

/* Complete zero-callee C translation of Pharaoh.exe 0x004B3320. */
int32_t pharaoh_message_count_empty_slots(const int16_t slots[1000])
{
    int32_t count = 0;
    for (int32_t index = 1; index < 1000; ++index)
        if (slots[index] == 0) ++count;
    return count;
}

/* Complete zero-callee C translation of Pharaoh.exe 0x004C7F70. */
int32_t pharaoh_message_input_hit_test(uint8_t active,
    const PharaohMessageInputRow *rows, int32_t count,
    int32_t mouse_x, int32_t mouse_y)
{
    if (active != 0) {
        for (int32_t index = 0; index < count; ++index) {
            const PharaohMessageInputRow *row = &rows[index];
            if (row->top <= mouse_y && mouse_y <= row->bottom &&
                row->left <= mouse_x && mouse_x <= row->right)
                return row->value;
        }
    }
    return -1;
}

void pharaoh_message_dialog_close(PharaohMessageDialogCloseState *state)
{
    if (state->show_video != 0) state->stop_video(state->user_data);
    state->show_video = 0;
    state->current_window = state->previous_window;
    state->message_category = 0;
    state->redraw_requested = 1;
}

void pharaoh_menu_message_ten(PharaohMenuMessageState *state)
{
    state->close_message_panel(state->user_data);
    state->show_message(state->user_data, 10, 0, 0);
}

void pharaoh_menu_message_go_to(PharaohMenuMessageState *state)
{
    PharaohMenuMessageRecord *event = &state->messages[state->selected_detail];
    const uint16_t category = state->message_category(state->user_data,
        state->active_message_id);
    uint32_t tile = event->second;
    if (category == 7u) {
        const uint32_t located = state->group_tile(state->user_data,
            (int32_t)event->first);
        if ((int32_t)located > 0) tile = located;
    }
    if ((int32_t)tile > 0 && (int32_t)tile < 0xcb10)
        state->go_to_tile(state->user_data, tile);
    state->current_screen = 1;
    state->redraw_requested = 1;
}

void pharaoh_menu_message_help(PharaohMenuMessageState *state)
{
    state->show_message(state->user_data, 34, 0, 0);
    state->redraw_requested = 1;
}

void pharaoh_menu_message_scroll(PharaohMenuMessageState *state)
{
    if (state->button_parameter == 0u) {
        if (state->scroll_index > 0) --state->scroll_index;
    } else if (state->button_parameter == 1u &&
        state->scroll_index < state->visible_count) {
        ++state->scroll_index;
    }
    state->scroll_reset = 0;
    state->redraw_requested = 1;
}

void pharaoh_menu_message_open(PharaohMenuMessageState *state)
{
    int32_t selected = (int32_t)((uint32_t)state->scroll_index +
        state->button_parameter);
    PharaohMenuMessageRecord *event;
    uint8_t templated;
    state->selected_message = selected;
    state->selected_detail = selected;
    if (selected >= state->message_count) return;
    event = &state->messages[selected];
    event->read = 1;
    state->message_kind = (uint32_t)(int32_t)event->kind;
    state->message_source = event->source;
    state->message_first = event->first;
    state->message_second = event->second;
    state->current_message = event->message;
    state->prepare_message(state->user_data, state->current_message);
    selected = state->selected_message;
    templated = state->messages[selected].template_id >= 0;
    state->show_message(state->user_data, state->current_message, 0,
        templated);
}

void pharaoh_menu_message_delete(PharaohMenuMessageState *state)
{
    const int32_t selected = (int32_t)((uint32_t)state->scroll_index +
        state->button_parameter);
    state->selected_message = selected;
    if (selected >= state->message_count) return;
    state->messages[selected].message = 0;
    state->sort_messages(state->user_data);
    if (state->scroll_index >= state->visible_count) --state->scroll_index;
    if (state->scroll_index < 0) state->scroll_index = 0;
    state->redraw_requested = 1;
}

void pharaoh_menu_message_delete_read(PharaohMenuMessageState *state)
{
    int32_t index;
    for (index = 0; index < 999; ++index) {
        PharaohMenuMessageRecord *event = &state->messages[index];
        const int16_t message = event->message;
        if (message != 0 && event->read != 0 &&
            state->message_category(state->user_data, message) != 4u) {
            event->message = 0;
        }
    }
    state->sort_messages(state->user_data);
    if (state->scroll_index >= state->visible_count)
        state->scroll_index = state->visible_count - 1;
    if (state->scroll_index < 0) state->scroll_index = 0;
    state->redraw_requested = 1;
}

void pharaoh_menu_overlay_select(PharaohMenuMessageState *state)
{
    const uint32_t value =
        state->overlay_values[state->overlay_group * 30u +
            state->button_parameter];
    if (value != 0u) {
        state->overlay_secondary = 0;
        state->current_overlay = value;
    }
    state->current_screen = 1;
    state->redraw_requested = 1;
    state->refresh_overlay(state->user_data);
}
