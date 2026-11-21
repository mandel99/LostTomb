#include "pharaoh/ui/load_save_actions.h"

#include <string.h>

static void copy_native_string(char *destination, const char *source)
{
    memcpy(destination, source, strlen(source) + 1u);
}

static void enter_dialog(PharaohLoadSaveActionsState *state, uint8_t mode)
{
    char pattern[260];
    copy_native_string(pattern, state->family_path);
    copy_native_string(pattern + strlen(pattern), state->save_suffix);
    state->scan_save_files(state->user_data, pattern, 0);
    state->text_edit_state = 0;
    state->current_screen = 13;
    state->return_screen = 1;
    state->dialog_mode = mode;
    state->clear_text(state->user_data, state->load_name, 64, 0);
    copy_native_string(state->load_name, state->default_name);
    state->prepare_text_editor(state->user_data, 2);
    state->prepare_text_cursor(state->user_data);
    state->prepare_text_selection(state->user_data);
    state->redraw_requested = 1;
}

void pharaoh_load_save_enter_load(PharaohLoadSaveActionsState *state)
{
    enter_dialog(state, 0);
}

void pharaoh_load_save_enter_alternate(PharaohLoadSaveActionsState *state)
{
    enter_dialog(state, 2);
}

void pharaoh_load_save_select_row(PharaohLoadSaveActionsState *state)
{
    char *destination;
    const char *name;
    int32_t index;
    if (state->selected_row >= state->file_count) return;
    state->selected_file = (uint32_t)state->selected_row;
    destination = state->current_screen == 13 ?
        state->load_name : state->alternate_name;
    state->clear_text(state->user_data, destination, 64, 0);
    index = (int32_t)(state->selected_file + state->first_visible_file);
    name = state->file_name(state->user_data, index);
    copy_native_string(destination, name);
    state->prepare_text_editor(state->user_data,
        state->current_screen == 13 ? 2 : 4);
    state->prepare_text_cursor(state->user_data);
    state->prepare_text_selection(state->user_data);
    state->selection_active = 0;
}
