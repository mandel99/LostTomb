#include "pharaoh/ui/family_options.h"

void pharaoh_family_action_cb800(PharaohFamilyOptionsState *state)
{
    state->stop_menu_sound(state->user_data);
    if (state->saved_game_available != 0u) {
        state->selected_family = 0;
        state->game_loaded = 0;
        state->load_saved_family(state->user_data);
        state->current_screen = 1;
    } else {
        state->transient_a = 0;
        state->transient_b = 0;
        state->mission_selection_pending = 0;
        state->current_screen = 0x17;
    }
    state->redraw_requested = 1;
}

void pharaoh_family_action_cb860(PharaohFamilyOptionsState *state)
{
    if (state->campaign_available == 0u) return;
    if (state->campaign_has_entries(state->user_data) == 0u) {
        const char *first = state->select_text(state->user_data, 5, 0x8d);
        const char *second = state->select_text(state->user_data, 5, 0x8e);
        if (state->run_text_dialog(state->user_data, first, second, 4) == 7)
            return;
    }
    if (state->campaign_resume_mode != 0u) {
        state->current_screen = 0x17;
        state->mission_selection_pending = 1;
    } else {
        state->selected_family = 0;
        state->game_loaded = 0;
        state->map_selection_mode = 2;
        state->map_selection_source = 1;
        state->show_map_selection(state->user_data);
    }
    state->redraw_requested = 1;
}

void pharaoh_family_action_cb910(PharaohFamilyOptionsState *state)
{
    if (state->editor_available == 0u) return;
    state->selected_family = 0;
    state->game_loaded = 0;
    state->prepare_editor_map_name(state->user_data);
    state->editor_next_window = 0x30;
}

void pharaoh_family_action_cb940(PharaohFamilyOptionsState *state)
{
    state->editor_mode = 2;
    state->selected_family = 0;
    state->game_loaded = 0;
    state->map_selection_source = 0;
    state->map_selection_mode = 3;
    state->show_map_selection(state->user_data);
}
