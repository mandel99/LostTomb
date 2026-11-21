#include "pharaoh/editor/god_buttons.h"

void pharaoh_editor_toggle_temple_complex(PharaohEditorGodState *state)
{
    const int32_t god = state->button_parameter;
    if (state->god_status[god] == 0) {
        state->temple_complex[god] = 0;
        *state->redraw_requested = 1;
        return;
    }

    const uint16_t current = state->temple_complex[god];
    if (current == 1) {
        state->temple_complex[god] = 0;
        *state->redraw_requested = 1;
        return;
    }
    if (current == 0) state->temple_complex[god] = 1;
    *state->redraw_requested = 1;
}

void pharaoh_editor_next_god_status(PharaohEditorGodState *state)
{
    const int32_t god = state->button_parameter;
    state->god_status[god] = (uint16_t)(state->god_status[god] + 1u);
    if ((int16_t)state->god_status[god] > 2) state->god_status[god] = 0;
    if (state->god_status[god] == 0) state->temple_complex[god] = 0;
    *state->redraw_requested = 1;
}
