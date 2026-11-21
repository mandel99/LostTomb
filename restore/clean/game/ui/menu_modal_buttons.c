#include "pharaoh/ui/menu_modal_buttons.h"

void pharaoh_menu_modal_open_27(PharaohMenuModalState *state)
{
    state->current_screen = 0x27;
    state->redraw_requested = 1;
}

void pharaoh_menu_modal_open_46(PharaohMenuModalState *state)
{
    state->current_screen = 0x46;
    state->redraw_requested = 1;
}

void pharaoh_menu_modal_open_2d(PharaohMenuModalState *state)
{
    state->current_screen = 0x2d;
    if (state->player->current > state->player->limit)
        state->player->current = state->player->limit;
    state->redraw_requested = 1;
}

void pharaoh_menu_modal_open_2c_or_4d(PharaohMenuModalState *state)
{
    if (state->player->gate == 0u) return;
    if (state->button_parameter == 0u) {
        state->current_screen = 0x2c;
        state->redraw_requested = 1;
        return;
    }
    state->saved_cursor_x = state->cursor_x;
    state->saved_cursor_y = state->cursor_y;
    state->reset_modal_state(state->user_data);
    state->current_screen = 0x4d;
    state->redraw_requested = 1;
}

void pharaoh_menu_modal_open_2e(PharaohMenuModalState *state)
{
    state->current_screen = 0x2e;
    if (state->player->price_mode == 2u &&
        state->player->price_mode_two > state->player->limit) {
        state->player->price_mode = 1;
    }
    if (state->player->price_mode == 1u &&
        state->player->price_mode_one > state->player->limit) {
        state->player->price_mode = 0;
    }
    state->redraw_requested = 1;
}

void pharaoh_menu_modal_close_trade(PharaohMenuModalState *state)
{
    if (state->current_screen == 0x2c) {
        state->current_screen = 7;
        state->redraw_requested = 1;
        return;
    }
    if (state->current_screen == 0x4d) state->current_screen = 9;
    state->redraw_requested = 1;
}
