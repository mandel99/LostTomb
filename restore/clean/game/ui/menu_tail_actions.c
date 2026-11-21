#include "pharaoh/ui/menu_tail_actions.h"

void pharaoh_menu_modal_choose(PharaohMenuTailState *state)
{
    state->modal_finished = 1;
    state->modal_result = state->button_parameter;
}

void pharaoh_menu_tail_buttons(PharaohMenuTailState *state)
{
    const uint32_t row = state->row_count * 16u - 40u;
    uint32_t advance = 0;
    uint32_t slot;
    state->button_scan_active = 1;
    for (slot = 0; slot < 15u; ++slot) {
        const void *buttons;
        uint32_t x;
        uint32_t y;
        if (state->slot_values[slot] == 0u || state->slot_enabled[slot] == 0u)
            continue;
        buttons = state->slot_buttons[slot];
        y = state->origin_y;
        x = state->origin_x;
        state->draw_button(state->user_data,
            (int32_t)(x + advance + 40u), (int32_t)(y + row),
            buttons, 1, (int32_t)(slot + 40u));
        buttons = state->slot_buttons[slot];
        y = state->origin_y;
        x = state->origin_x;
        if (state->handle_button(state->user_data,
                (int32_t)(x + advance + 40u), (int32_t)(y + row),
                buttons, 1) != 0) {
            return;
        }
        advance += 27u;
    }
    state->button_scan_complete = 0;
}

void pharaoh_menu_tail_player_widget(PharaohMenuTailState *state)
{
    const PharaohMenuTailPlayer *player =
        &state->players[state->current_player];
    int32_t flags = 0;
    if (player->hidden != 0u) return;
    if (player->flag_option != 0u) flags = state->focused_button == 1u;
    state->draw_layout_widget(state->user_data, 0,
        (int32_t)(state->origin_x + 82u),
        (int32_t)(state->origin_y + 153u), 300, 20, flags,
        (int32_t)state->origin_x, (int32_t)state->origin_y);
}
