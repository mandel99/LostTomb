#include "pharaoh/runtime/text_input_state.h"

void pharaoh_text_input_field_initialize(PharaohTextInputState *state,
    int32_t field_id, char *text_buffer, int32_t max_length, int32_t width,
    uint8_t allow_space, uint32_t font_id)
{
    PharaohTextInputField *const field = &state->fields[field_id];
    field->text_buffer = text_buffer;
    field->max_length = max_length;
    field->width = width;
    field->allow_space = allow_space;
    field->font_id = font_id;
    field->state_a = 0;
    field->state_b = 0;
    state->shared_state_a = 0;
    state->shared_state_b = 0;
}
