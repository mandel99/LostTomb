#ifndef PHARAOH_EDITOR_OPTION_BUTTONS_H
#define PHARAOH_EDITOR_OPTION_BUTTONS_H

#include "pharaoh/editor/numeric_dialog.h"

#include <stdint.h>

typedef struct PharaohEditorOptionState {
    uint16_t checkbox;
    uint8_t byte_value;
    int32_t dword_value;
    int32_t selection_value;
    uint8_t *toggle_flags;
    int32_t *indexed_values;
    int32_t button_parameter;
    int32_t exclusive_first;
    int32_t exclusive_first_value;
    int32_t exclusive_second;
    int32_t exclusive_second_value;
    int32_t enabled;
    int32_t enabled_value;
    uint8_t redraw_requested;
    PharaohEditorNumericDialog *numeric_dialog;
    void (*open_selection)(void *user_data, int32_t x, int32_t y,
        int32_t rows, int32_t text_id, int32_t *target);
    void *user_data;
} PharaohEditorOptionState;

void pharaoh_editor_option_toggle_checkbox(PharaohEditorOptionState *state);
void pharaoh_editor_option_edit_byte(PharaohEditorOptionState *state);
void pharaoh_editor_option_edit_dword(PharaohEditorOptionState *state);
void pharaoh_editor_option_choose_value(PharaohEditorOptionState *state);
void pharaoh_editor_option_toggle_indexed(PharaohEditorOptionState *state);
void pharaoh_editor_option_edit_indexed(PharaohEditorOptionState *state);
void pharaoh_editor_option_toggle_first(PharaohEditorOptionState *state);
void pharaoh_editor_option_edit_first(PharaohEditorOptionState *state);
void pharaoh_editor_option_toggle_second(PharaohEditorOptionState *state);
void pharaoh_editor_option_edit_second(PharaohEditorOptionState *state);
void pharaoh_editor_option_toggle_enabled(PharaohEditorOptionState *state);
void pharaoh_editor_option_edit_enabled(PharaohEditorOptionState *state);

#endif
