#ifndef PHARAOH_RUNTIME_TEXT_INPUT_STATE_H
#define PHARAOH_RUNTIME_TEXT_INPUT_STATE_H

#include <stdint.h>

enum { PHARAOH_TEXT_INPUT_FIELD_SLOTS = 8 };

typedef struct PharaohTextInputField {
    char *text_buffer;
    int32_t max_length;
    int32_t width;
    uint8_t allow_space;
    uint32_t font_id;
    int32_t state_a;
    int32_t state_b;
} PharaohTextInputField;

typedef struct PharaohTextInputState {
    PharaohTextInputField fields[PHARAOH_TEXT_INPUT_FIELD_SLOTS];
    int32_t shared_state_a;
    int32_t shared_state_b;
} PharaohTextInputState;

void pharaoh_text_input_field_initialize(PharaohTextInputState *state,
    int32_t field_id, char *text_buffer, int32_t max_length, int32_t width,
    uint8_t allow_space, uint32_t font_id);

#endif
