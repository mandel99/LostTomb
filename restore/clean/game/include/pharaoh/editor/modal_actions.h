#ifndef PHARAOH_EDITOR_MODAL_ACTIONS_H
#define PHARAOH_EDITOR_MODAL_ACTIONS_H

#include <stdint.h>

#include "pharaoh/editor/popup_actions.h"

typedef struct PharaohEditorModalState {
    uint32_t window_x;
    uint32_t window_y;
    int32_t first_number;
    int32_t second_number;
    int32_t third_number;
    int16_t word_choice;
    uint8_t palette;
    uint8_t option_enabled;
    uint8_t redraw_requested;
    void (*open_number)(void *user_data, int32_t x, int32_t y,
        int32_t digits, int32_t minimum, int32_t maximum, int32_t *target,
        int32_t completion);
    void (*open_selection)(void *user_data,
        const PharaohSelectionPopupRequest *request);
    void (*apply_palette)(void *user_data, uint8_t palette);
    void *user_data;
} PharaohEditorModalState;

void pharaoh_editor_open_first_number(PharaohEditorModalState *state);
void pharaoh_editor_open_second_number(PharaohEditorModalState *state);
void pharaoh_editor_open_third_number(PharaohEditorModalState *state);
void pharaoh_editor_open_word_selection(PharaohEditorModalState *state);
void pharaoh_editor_cycle_palette(PharaohEditorModalState *state);
void pharaoh_editor_toggle_option(PharaohEditorModalState *state);
uint32_t pharaoh_editor_apply_palette(PharaohEditorModalState *state,
    uint32_t palette);

#endif
