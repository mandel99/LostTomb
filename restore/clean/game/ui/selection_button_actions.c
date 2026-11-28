#include "pharaoh/ui/selection_button_actions.h"

enum { PLAYER_RECORD_SIZE = 196 };

static void mark_offsets(const PharaohSelectionButtonBorrowedState *state,
    uint32_t count)
{
    uint32_t index;
    for (index = 0; index < count; ++index)
        state->resource_states[state->offsets[index]] = 1;
}

void pharaoh_selection_toggle_borrowed(const PharaohSelectionButtonBorrowedState *state)
{
    const uint32_t previous = *state->toggle;
    *state->toggle = previous != 1u ? 1u : 0u;
    *state->redraw_requested = 1;
}

void pharaoh_selection_enable_mode_borrowed(const PharaohSelectionButtonBorrowedState *state)
{
    *state->selection_mode = 1;
    *state->redraw_requested = 1;
}

void pharaoh_selection_mark_primary_borrowed(const PharaohSelectionButtonBorrowedState *state)
{
    const int32_t count = *state->primary_count;
    if (count > 0)mark_offsets(state,(uint32_t)count);
    *state->redraw_requested = 1;
}

void pharaoh_selection_apply_borrowed(const PharaohSelectionButtonBorrowedState *state)
{
    const uint32_t parameter = *state->button_parameter;
    if (parameter == 1u) {
        *state->player_selected_building = *state->selected_building;
    } else if (parameter == 2u) {
        const int32_t count = *state->secondary_count;
        if(count > 0)mark_offsets(state,(uint32_t)count);
    }
    *state->redraw_requested = 1;
}

void pharaoh_selection_toggle(PharaohSelectionButtonState *state)
{
    PharaohSelectionButtonBorrowedState borrowed={0};
    borrowed.toggle=&state->toggle;borrowed.redraw_requested=&state->redraw_requested;
    pharaoh_selection_toggle_borrowed(&borrowed);
}

void pharaoh_selection_enable_mode(PharaohSelectionButtonState *state)
{
    PharaohSelectionButtonBorrowedState borrowed={0};
    borrowed.selection_mode=&state->selection_mode;borrowed.redraw_requested=&state->redraw_requested;
    pharaoh_selection_enable_mode_borrowed(&borrowed);
}

void pharaoh_selection_mark_primary(PharaohSelectionButtonState *state)
{
    const uint32_t owner=state->building_owner[state->selected_building];
    PharaohSelectionButtonBorrowedState borrowed={0};
    borrowed.primary_count=&state->primary_count;borrowed.offsets=state->offsets;
    if(state->primary_count>0)borrowed.resource_states=state->player_records+owner*PLAYER_RECORD_SIZE;
    borrowed.redraw_requested=&state->redraw_requested;
    pharaoh_selection_mark_primary_borrowed(&borrowed);
}

void pharaoh_selection_apply(PharaohSelectionButtonState *state)
{
    const uint32_t owner=state->building_owner[state->selected_building];
    PharaohSelectionButtonBorrowedState borrowed={0};
    borrowed.selected_building=&state->selected_building;
    borrowed.button_parameter=&state->button_parameter;
    borrowed.secondary_count=&state->secondary_count;borrowed.offsets=state->offsets;
    if(state->button_parameter==1u)
        borrowed.player_selected_building=&state->player_selected_building[state->current_player];
    else if(state->button_parameter==2u&&state->secondary_count>0)
        borrowed.resource_states=state->player_records+owner*PLAYER_RECORD_SIZE;
    borrowed.redraw_requested=&state->redraw_requested;
    pharaoh_selection_apply_borrowed(&borrowed);
}
