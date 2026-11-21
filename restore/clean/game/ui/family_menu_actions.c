#include "pharaoh/ui/family_menu_actions.h"

#include <string.h>

void pharaoh_family_open_name_choices(PharaohFamilyMenuState *state)
{
    state->current_screen = 0x51;
    state->redraw_requested = 1;
}

void pharaoh_family_close_name_choices(PharaohFamilyMenuState *state)
{
    state->text_edit_state = 0;
    state->current_screen = 0x36;
    state->redraw_requested = 1;
}

void pharaoh_family_choose_name(PharaohFamilyMenuState *state)
{
    const char *name = state->select_text(state->user_data, 197,
        (int32_t)state->selected_row);
    memcpy(state->family_name, name, strlen(name) + 1u);
    state->prepare_text_editor(state->user_data, 1);
    state->prepare_text_cursor(state->user_data);
    state->prepare_text_selection(state->user_data);
    state->highlighted_row = state->selected_row;
}

void pharaoh_family_begin_new(PharaohFamilyMenuState *state)
{
    memset(state->family_name, 0, sizeof(state->family_name));
    state->current_screen = 0x36;
    state->text_edit_state = 0;
    state->prepare_text_editor(state->user_data, 1);
    state->prepare_text_cursor(state->user_data);
    state->prepare_text_selection(state->user_data);
    state->input_window = 0x2f;
    state->redraw_requested = 1;
}

void pharaoh_family_request_delete(PharaohFamilyMenuState *state)
{
    state->show_dialog(state->user_data, 0x5a,
        PHARAOH_FAMILY_DIALOG_DELETE_DYNASTY, 2);
}
