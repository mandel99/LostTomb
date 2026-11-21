#include "pharaoh/ui/selection_button_actions.h"

enum { PLAYER_RECORD_SIZE = 196 };

static void mark_offsets(PharaohSelectionButtonState *state,
    uint32_t player_offset, uint32_t count)
{
    uint32_t index;
    for (index = 0; index < count; ++index)
        state->player_records[player_offset + state->offsets[index]] = 1;
}

void pharaoh_selection_toggle(PharaohSelectionButtonState *state)
{
    const uint32_t previous = state->toggle;
    state->toggle = previous != 1u ? 1u : 0u;
    state->redraw_requested = 1;
}

void pharaoh_selection_enable_mode(PharaohSelectionButtonState *state)
{
    state->selection_mode = 1;
    state->redraw_requested = 1;
}

void pharaoh_selection_mark_primary(PharaohSelectionButtonState *state)
{
    const uint32_t owner = state->building_owner[state->selected_building];
    const uint32_t count = (uint32_t)state->primary_count;
    if (state->primary_count > 0)
        mark_offsets(state, owner * PLAYER_RECORD_SIZE, count);
    state->redraw_requested = 1;
}

void pharaoh_selection_apply(PharaohSelectionButtonState *state)
{
    const uint32_t owner = state->building_owner[state->selected_building];
    if (state->button_parameter == 1u) {
        state->player_selected_building[state->current_player] =
            state->selected_building;
    } else if (state->button_parameter == 2u && state->secondary_count > 0) {
        mark_offsets(state, owner * PLAYER_RECORD_SIZE,
            (uint32_t)state->secondary_count);
    }
    state->redraw_requested = 1;
}
