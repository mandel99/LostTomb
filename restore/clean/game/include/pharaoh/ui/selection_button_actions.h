#ifndef PHARAOH_UI_SELECTION_BUTTON_ACTIONS_H
#define PHARAOH_UI_SELECTION_BUTTON_ACTIONS_H

#include <stdint.h>

typedef struct PharaohSelectionButtonState {
    uint32_t toggle;
    uint32_t selection_mode;
    uint32_t selected_building;
    uint32_t button_parameter;
    uint32_t current_player;
    int32_t primary_count;
    int32_t secondary_count;
    const uint32_t *offsets;
    const uint8_t *building_owner;
    uint8_t *player_records;
    uint32_t *player_selected_building;
    uint8_t redraw_requested;
} PharaohSelectionButtonState;

void pharaoh_selection_toggle(PharaohSelectionButtonState *state);
void pharaoh_selection_enable_mode(PharaohSelectionButtonState *state);
void pharaoh_selection_mark_primary(PharaohSelectionButtonState *state);
void pharaoh_selection_apply(PharaohSelectionButtonState *state);

#endif
