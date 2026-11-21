#ifndef PHARAOH_UI_TABLE_ACTIONS_H
#define PHARAOH_UI_TABLE_ACTIONS_H

#include <stdint.h>

typedef struct PharaohMenuTablePlayer {
    uint32_t value_0;
    uint32_t value_1;
    uint32_t value_2;
    uint32_t adjustable;
} PharaohMenuTablePlayer;

typedef struct PharaohMenuTableState {
    PharaohMenuTablePlayer *players;
    uint32_t current_player;
    uint32_t selected_value;
    uint32_t global_value;
    uint8_t current_screen;
    uint8_t redraw_requested;
    void (*refresh_table)(void *user_data);
    void (*run_secondary_action)(void *user_data);
    void *user_data;
} PharaohMenuTableState;

void pharaoh_menu_table_reset(PharaohMenuTableState *state);
void pharaoh_menu_table_secondary(PharaohMenuTableState *state);
void pharaoh_menu_table_decrement(PharaohMenuTableState *state);

#endif
