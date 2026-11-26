#ifndef PHARAOH_RUNTIME_TEXT_INPUT_STATE_H
#define PHARAOH_RUNTIME_TEXT_INPUT_STATE_H

#include <stdint.h>

#include "pharaoh/ui/bitmap_font.h"

enum { PHARAOH_TEXT_INPUT_FIELD_SLOTS = 20 };

typedef struct PharaohTextInputField {
    char *text_buffer;
    int32_t max_length;
    int32_t width;
    uint8_t restrict_punctuation;
    uint32_t font_id;
} PharaohTextInputField;

typedef struct PharaohTextInputState {
    PharaohTextInputField fields[PHARAOH_TEXT_INPUT_FIELD_SLOTS];
    int32_t cursor_positions[PHARAOH_TEXT_INPUT_FIELD_SLOTS];
    int32_t text_lengths[PHARAOH_TEXT_INPUT_FIELD_SLOTS];
    int32_t active_field;
    uint8_t insert_mode;
    int32_t return_pressed;
    PharaohBitmapTextDrawState drawing; /* actual shared cursor/anchor globals */
    int32_t capture_limit_state;
    const PharaohBitmapFont *(*resolve_font)(void *user_data, uint32_t font_id);
    void *font_user_data;
} PharaohTextInputState;

/* Bind the renderer to these actual native cursor positions and active slot. */
void pharaoh_text_input_bind_drawing(PharaohTextInputState *state);

void pharaoh_text_input_field_initialize(PharaohTextInputState *state,
    int32_t field_id, char *text_buffer, int32_t max_length, int32_t width,
    uint8_t restrict_punctuation, uint32_t font_id);
void pharaoh_keyboard_return(PharaohTextInputState *state);
void pharaoh_keyboard_insert(PharaohTextInputState *state);
void pharaoh_keyboard_left(PharaohTextInputState *state);
void pharaoh_keyboard_home(PharaohTextInputState *state);
void pharaoh_keyboard_end(PharaohTextInputState *state);
void pharaoh_text_cursor_reset(PharaohTextInputState *state);
void pharaoh_text_insert_gap(char *start, char *cursor);
void pharaoh_keyboard_right(PharaohTextInputState *state);
uint8_t pharaoh_text_next_character(const char *text, int32_t *index);
void pharaoh_text_input_activate(
    PharaohTextInputState *state, int32_t field_id);
void pharaoh_text_input_update_cursor(PharaohTextInputState *state);
void pharaoh_text_input_insert(
    PharaohTextInputState *state, uint8_t character);
void pharaoh_keyboard_character(
    PharaohTextInputState *state, uint8_t character);
void pharaoh_keyboard_backspace(PharaohTextInputState *state);
void pharaoh_keyboard_delete(PharaohTextInputState *state);
void pharaoh_keyboard_delete_thunk(PharaohTextInputState *state);

#endif
