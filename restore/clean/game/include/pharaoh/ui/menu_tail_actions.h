#ifndef PHARAOH_UI_MENU_TAIL_ACTIONS_H
#define PHARAOH_UI_MENU_TAIL_ACTIONS_H

#include <stdint.h>

typedef struct PharaohMenuTailPlayer {
    uint32_t hidden;
    uint16_t flag_option;
} PharaohMenuTailPlayer;

typedef struct PharaohMenuTailState {
    uint32_t button_parameter;
    uint8_t modal_finished;
    uint32_t modal_result;
    uint32_t row_count;
    uint32_t origin_x;
    uint32_t origin_y;
    const uint32_t *slot_values;
    const uint8_t *slot_enabled;
    const void *const *slot_buttons;
    uint8_t button_scan_active;
    uint8_t button_scan_complete;
    PharaohMenuTailPlayer *players;
    uint32_t current_player;
    uint32_t focused_button;
    void (*draw_button)(void *user_data, int32_t x, int32_t y,
        const void *button, int32_t count, int32_t id);
    int32_t (*handle_button)(void *user_data, int32_t x, int32_t y,
        const void *button, int32_t count);
    void (*draw_layout_widget)(void *user_data, int32_t style, int32_t x,
        int32_t y, int32_t width, int32_t height, int32_t flags,
        int32_t origin_x, int32_t origin_y);
    void *user_data;
} PharaohMenuTailState;

void pharaoh_menu_modal_choose(PharaohMenuTailState *state);
void pharaoh_menu_tail_buttons(PharaohMenuTailState *state);
void pharaoh_menu_tail_player_widget(PharaohMenuTailState *state);

#endif
