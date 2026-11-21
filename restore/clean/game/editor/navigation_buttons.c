#include "pharaoh/editor/navigation_buttons.h"

static void open_screen(PharaohEditorNavigationState *state, uint8_t screen)
{
    *state->current_screen = screen;
    *state->redraw_requested = 1;
}

void pharaoh_editor_open_flood(PharaohEditorNavigationState *state)
{
    open_screen(state, 0x19);
}

void pharaoh_editor_open_allowed_buildings(
    PharaohEditorNavigationState *state)
{
    open_screen(state, 0x1b);
}

void pharaoh_editor_open_win_criteria(PharaohEditorNavigationState *state)
{
    open_screen(state, 0x1c);
}

void pharaoh_editor_open_settings(PharaohEditorNavigationState *state)
{
    open_screen(state, 0x25);
}

void pharaoh_editor_settings_milestone(PharaohEditorNavigationState *state,
    const PharaohEditorNavigationOperations *operations)
{
    operations->open_numeric_input(operations->user_data, 300, 100, 4,
        &state->milestones[state->button_parameter]);
    *state->redraw_requested = 1;
}

void pharaoh_editor_settings_start_year(
    PharaohEditorNavigationState *state)
{
    open_screen(state, 0x16);
}
