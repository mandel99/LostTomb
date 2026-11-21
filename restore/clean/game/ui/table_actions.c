#include "pharaoh/ui/table_actions.h"

void pharaoh_menu_table_reset(PharaohMenuTableState *state)
{
    PharaohMenuTablePlayer *player = &state->players[state->current_player];
    player->value_0 = 0;
    player->value_1 = 0;
    player->value_2 = 0;
    state->refresh_table(state->user_data);
    state->global_value = 0;
    state->redraw_requested = 1;
}

void pharaoh_menu_table_secondary(PharaohMenuTableState *state)
{
    state->run_secondary_action(state->user_data);
}

void pharaoh_menu_table_decrement(PharaohMenuTableState *state)
{
    PharaohMenuTablePlayer *player = &state->players[state->current_player];
    uint32_t value = player->adjustable - 1u;
    state->selected_value = value;
    if ((int32_t)value > 1) {
        value = 1;
        state->selected_value = value;
    }
    player->adjustable = value;
    state->current_screen = 1;
    state->redraw_requested = 1;
}
