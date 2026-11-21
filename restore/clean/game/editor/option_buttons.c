#include "pharaoh/editor/option_buttons.h"

enum { INPUT_X = 300, INPUT_Y = 200, SELECTION_Y = 100 };

static void redraw(PharaohEditorOptionState *state)
{
    state->redraw_requested = 1;
}

static void edit_dword(PharaohEditorOptionState *state, int32_t *target)
{
    pharaoh_editor_numeric_show(state->numeric_dialog, INPUT_X, INPUT_Y,
        PHARAOH_EDITOR_NUMERIC_INT32, 0, 0, target,
        PHARAOH_EDITOR_NUMERIC_PLAIN);
    redraw(state);
}

void pharaoh_editor_option_toggle_checkbox(PharaohEditorOptionState *state)
{
    redraw(state);
    state->checkbox = (uint16_t)(state->checkbox == 0u);
}

void pharaoh_editor_option_edit_byte(PharaohEditorOptionState *state)
{
    pharaoh_editor_numeric_show(state->numeric_dialog, INPUT_X, INPUT_Y,
        PHARAOH_EDITOR_NUMERIC_INT8, &state->byte_value, 0, 0,
        PHARAOH_EDITOR_NUMERIC_PLAIN);
    redraw(state);
}

void pharaoh_editor_option_edit_dword(PharaohEditorOptionState *state)
{
    edit_dword(state, &state->dword_value);
}

void pharaoh_editor_option_choose_value(PharaohEditorOptionState *state)
{
    state->open_selection(state->user_data, INPUT_X, SELECTION_Y, 20, 29,
        &state->selection_value);
    redraw(state);
}

void pharaoh_editor_option_toggle_indexed(PharaohEditorOptionState *state)
{
    redraw(state);
    uint8_t *value = &state->toggle_flags[state->button_parameter];
    *value = (uint8_t)(*value == 0u);
}

void pharaoh_editor_option_edit_indexed(PharaohEditorOptionState *state)
{
    edit_dword(state, &state->indexed_values[state->button_parameter]);
}

void pharaoh_editor_option_toggle_first(PharaohEditorOptionState *state)
{
    if (state->exclusive_first != 0) {
        state->exclusive_first = 0;
        redraw(state);
        return;
    }
    state->exclusive_second = 0;
    state->exclusive_first = 1;
    redraw(state);
}

void pharaoh_editor_option_edit_first(PharaohEditorOptionState *state)
{
    edit_dword(state, &state->exclusive_first_value);
}

void pharaoh_editor_option_toggle_second(PharaohEditorOptionState *state)
{
    if (state->exclusive_second != 0) {
        state->exclusive_second = 0;
        redraw(state);
        return;
    }
    state->exclusive_first = 0;
    state->exclusive_second = 1;
    redraw(state);
}

void pharaoh_editor_option_edit_second(PharaohEditorOptionState *state)
{
    edit_dword(state, &state->exclusive_second_value);
}

void pharaoh_editor_option_toggle_enabled(PharaohEditorOptionState *state)
{
    state->enabled = state->enabled == 0 ? 1 : 0;
    redraw(state);
}

void pharaoh_editor_option_edit_enabled(PharaohEditorOptionState *state)
{
    edit_dword(state, &state->enabled_value);
}
