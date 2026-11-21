#include "pharaoh/editor/modal_actions.h"

#include <stddef.h>

static void open_number(PharaohEditorModalState *state, int32_t *target)
{
    state->open_number(state->user_data,
        (int32_t)(state->window_x + 0x8cu),
        (int32_t)(state->window_y + 0x38u), 4, 0, 0, target, 0);
    state->redraw_requested = 1;
}

void pharaoh_editor_open_first_number(PharaohEditorModalState *state)
{
    open_number(state, &state->first_number);
}

void pharaoh_editor_open_second_number(PharaohEditorModalState *state)
{
    open_number(state, &state->second_number);
}

void pharaoh_editor_open_third_number(PharaohEditorModalState *state)
{
    open_number(state, &state->third_number);
}

void pharaoh_editor_open_word_selection(PharaohEditorModalState *state)
{
    const PharaohSelectionPopupRequest request = {
        20, 40, 12, 37, 0, NULL, &state->word_choice, NULL,
        0, 2, 0, 0, 0, -1, 0
    };
    state->open_selection(state->user_data, &request);
    state->redraw_requested = 1;
}

uint32_t pharaoh_editor_apply_palette(PharaohEditorModalState *state,
    uint32_t palette)
{
    state->palette = (uint8_t)palette;
    state->apply_palette(state->user_data, state->palette);
    return 0;
}

void pharaoh_editor_cycle_palette(PharaohEditorModalState *state)
{
    uint8_t next = (uint8_t)(state->palette + 1u);
    state->palette = next;
    if (next >= 3u) state->palette = 0;
    pharaoh_editor_apply_palette(state, state->palette);
    state->redraw_requested = 1;
}

void pharaoh_editor_toggle_option(PharaohEditorModalState *state)
{
    const uint8_t previous = state->option_enabled;
    state->redraw_requested = 1;
    state->option_enabled = previous != 1u ? 1u : 0u;
}
