#include "pharaoh/ui/load_save_actions.h"
#include "pharaoh/ui/player_selection.h"
#include "pharaoh/core/memory_fill.h"

#include <string.h>

static void copy_native_string(char *destination, const char *source)
{
    memcpy(destination, source, strlen(source) + 1u);
}

static void enter_dialog(PharaohLoadSaveActionsState *state, uint8_t mode)
{
    char pattern[260];
    copy_native_string(pattern, pharaoh_player_directory_path(
        state->selector->family_directory, state->selector->player_name));
    copy_native_string(pattern + strlen(pattern), "\\*.sav");
    /* 004CDFF0 ignores the supplied pattern; it rebuilds it and publishes
       the native family-directory output again before scanning. */
    pharaoh_file_selector_initialize(state->selector, 0);
    state->text_input->return_pressed = 0;
    *state->current_screen = 13;
    *state->return_screen = 1;
    *state->dialog_mode = mode;
    pharaoh_memory_fill_bytes(state->load_name, 64, 0);
    copy_native_string((char *)state->load_name, state->default_name);
    pharaoh_text_input_activate(state->text_input, 2);
    pharaoh_keyboard_home(state->text_input);
    pharaoh_keyboard_end(state->text_input);
    *state->redraw_requested = 1;
}

void pharaoh_load_save_enter_load(PharaohLoadSaveActionsState *state)
{
    *state->load_latch = 0;
    enter_dialog(state, 1);
}

void pharaoh_load_save_enter_save(PharaohLoadSaveActionsState *state)
{
    enter_dialog(state, 0);
}

void pharaoh_load_save_enter_delete(PharaohLoadSaveActionsState *state)
{
    enter_dialog(state, 2);
}

void pharaoh_load_save_select_row(PharaohLoadSaveActionsState *state)
{
    char *destination;
    const char *name;
    const uint32_t selected = *state->button_parameter;
    const int32_t row = selected <= INT32_MAX ? (int32_t)selected :
        (int32_t)((int64_t)selected - INT64_C(0x100000000));
    if (row >= *state->selector->file_count) return;
    *state->selector->selected_file = selected;
    uint32_t *const words = *state->current_screen == 13 ?
        state->load_name : state->alternate_name;
    destination = (char *)words;
    pharaoh_memory_fill_bytes(words, 64, 0);
    const uint32_t bits = *state->selector->selected_file +
        *state->selector->first_visible;
    const int32_t index = bits <= INT32_MAX ? (int32_t)bits :
        (int32_t)((int64_t)bits - INT64_C(0x100000000));
    name = pharaoh_file_list_name(state->selector->files, index);
    copy_native_string(destination, name);
    pharaoh_text_input_activate(state->text_input,
        *state->current_screen == 13 ? 2 : 4);
    pharaoh_keyboard_home(state->text_input);
    pharaoh_keyboard_end(state->text_input);
    *state->selector->selection_active = 0;
}
