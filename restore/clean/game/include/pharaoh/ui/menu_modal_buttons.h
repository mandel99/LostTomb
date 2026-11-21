#ifndef PHARAOH_UI_MENU_MODAL_BUTTONS_H
#define PHARAOH_UI_MENU_MODAL_BUTTONS_H

#include <stdint.h>

typedef struct PharaohMenuModalPlayer {
    int32_t limit;
    int32_t current;
    uint32_t gate;
    uint32_t price_mode;
    int32_t price_mode_one;
    int32_t price_mode_two;
} PharaohMenuModalPlayer;

typedef struct PharaohMenuModalState {
    uint8_t current_screen;
    uint8_t redraw_requested;
    uint32_t button_parameter;
    uint32_t cursor_x;
    uint32_t cursor_y;
    uint32_t saved_cursor_x;
    uint32_t saved_cursor_y;
    PharaohMenuModalPlayer *player;
    void (*reset_modal_state)(void *user_data);
    void *user_data;
} PharaohMenuModalState;

void pharaoh_menu_modal_open_27(PharaohMenuModalState *state);
void pharaoh_menu_modal_open_46(PharaohMenuModalState *state);
void pharaoh_menu_modal_open_2d(PharaohMenuModalState *state);
void pharaoh_menu_modal_open_2c_or_4d(PharaohMenuModalState *state);
void pharaoh_menu_modal_open_2e(PharaohMenuModalState *state);
void pharaoh_menu_modal_close_trade(PharaohMenuModalState *state);

#endif
