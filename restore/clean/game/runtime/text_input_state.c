#include "pharaoh/runtime/text_input_state.h"

#include "pharaoh/core/memory_strings.h"

void pharaoh_text_input_bind_drawing(PharaohTextInputState *state)
{
    state->drawing.cursor_slot = &state->active_field;
    state->drawing.cursor_limits = state->cursor_positions;
}

void pharaoh_text_input_field_initialize(PharaohTextInputState *state,
    int32_t field_id, char *text_buffer, int32_t max_length, int32_t width,
    uint8_t restrict_punctuation, uint32_t font_id)
{
    PharaohTextInputField *const field = &state->fields[field_id];
    field->text_buffer = text_buffer;
    field->max_length = max_length;
    field->width = width;
    field->restrict_punctuation = restrict_punctuation;
    field->font_id = font_id;
    state->text_lengths[field_id] = 0;
    state->cursor_positions[field_id] = 0;
    state->capture_limit_state = 0;
    state->drawing.anchor_y = 0;
}

/* Complete C translations of Pharaoh.exe 0x004CC320, 0x004CC360,
   0x004CC370, 0x004CC3C0, 0x004CC3E0 and 0x004CC540. */
void pharaoh_keyboard_return(PharaohTextInputState *state)
{
    state->return_pressed = 1;
}

void pharaoh_keyboard_insert(PharaohTextInputState *state)
{
    state->insert_mode ^= 1u;
}

void pharaoh_keyboard_left(PharaohTextInputState *state)
{
    int32_t *position = &state->cursor_positions[state->active_field];
    if (*position > 0) --*position;
}

void pharaoh_keyboard_home(PharaohTextInputState *state)
{
    state->cursor_positions[state->active_field] = 0;
}

void pharaoh_keyboard_end(PharaohTextInputState *state)
{
    state->cursor_positions[state->active_field] =
        state->text_lengths[state->active_field];
}

void pharaoh_text_cursor_reset(PharaohTextInputState *state)
{
    state->drawing.anchor_captured = 0;
    state->drawing.character_index = 0;
    state->drawing.anchor_advance = 0;
    state->drawing.cursor_x = 0;
    state->drawing.cursor_y = 0;
}

/* Complete C translation of Pharaoh.exe 0x004CBF10. */
void pharaoh_text_insert_gap(char *start, char *cursor)
{
    cursor[1] = 0;
    while (start < cursor) {
        *cursor = cursor[-1];
        --cursor;
    }
}

/* Complete C translation of Pharaoh.exe 0x004CC390. */
void pharaoh_keyboard_right(PharaohTextInputState *state)
{
    int32_t *position = &state->cursor_positions[state->active_field];
    if (*position < state->text_lengths[state->active_field]) ++*position;
}

/* Complete logical-byte translation of Pharaoh.exe 0x004CCB00. */
uint8_t pharaoh_text_next_character(const char *text, int32_t *index)
{
    const uint8_t value = (uint8_t)text[*index];
    if (value == 0) {
        *index = 0;
        return 0x20;
    }
    ++*index;
    return value;
}

/* Complete C translation of Pharaoh.exe 0x004CC500. */
void pharaoh_text_input_activate(
    PharaohTextInputState *state, int32_t field_id)
{
    int32_t index = 0;
    int32_t *length = &state->text_lengths[field_id];
    state->active_field = field_id;
    *length = 0;
    if (state->fields[field_id].max_length >= 0) {
        do {
            if (state->fields[field_id].text_buffer[index] == 0) return;
            ++index;
            ++*length;
        } while (index <= state->fields[field_id].max_length);
    }
}

/* Complete one-callee C translation of Pharaoh.exe 0x004CC1B0. */
void pharaoh_text_input_update_cursor(PharaohTextInputState *state)
{
    const int32_t field_id = state->active_field;
    int32_t *cursor;
    int32_t measured_width;

    state->capture_limit_state = 0;
    if (field_id <= 0) return;

    const PharaohBitmapFont *const font = state->resolve_font(
        state->font_user_data, state->fields[field_id].font_id);
    measured_width = pharaoh_bitmap_text_width_native(font,
        (const uint8_t *)state->fields[field_id].text_buffer,
        pharaoh_bitmap_native_glyph_map(), font->current_glyph);
    cursor = &state->cursor_positions[field_id];
    if (state->text_lengths[field_id] + 1 <= *cursor) {
        *cursor = state->text_lengths[field_id] + 1;
        state->capture_limit_state = 1;
    }
    if (state->fields[field_id].max_length <= *cursor) {
        *cursor = state->fields[field_id].max_length;
        state->capture_limit_state = 2;
    }
    if (state->fields[field_id].width <= measured_width)
        state->capture_limit_state = 2;
    if (*cursor < 0) *cursor = 0;
}

/* Complete two-callee C translation of Pharaoh.exe 0x004CC400. */
void pharaoh_text_input_insert(
    PharaohTextInputState *state, uint8_t character)
{
    const int32_t field_id = state->active_field;
    PharaohTextInputField *field;
    int32_t cursor;

    if (field_id <= 0) return;
    pharaoh_text_input_update_cursor(state);
    if (state->capture_limit_state == 2) return;

    field = &state->fields[field_id];
    cursor = state->cursor_positions[field_id];
    if (state->insert_mode == 0) {
        field->text_buffer[cursor] = (char)character;
        state->cursor_positions[field_id] = cursor + 1;
        field->text_buffer[cursor + 1] = 0;
    } else if (state->text_lengths[field_id] < field->max_length) {
        pharaoh_text_insert_gap(field->text_buffer + cursor,
            field->text_buffer + state->text_lengths[field_id] + 1);
        field->text_buffer[cursor] = (char)character;
        state->cursor_positions[field_id] = cursor + 1;
    }
}

/* Complete two-callee C translation of Pharaoh.exe 0x004CC250. */
void pharaoh_keyboard_character(
    PharaohTextInputState *state, uint8_t character)
{
    uint8_t accepted = 0;
    const uint8_t restricted =
        state->fields[state->active_field].restrict_punctuation;

    if (character == ' ' || character == '-') accepted = 1;
    else if (character == ',' || character == '.' ||
             character == '?' || character == '!') accepted = !restricted;
    else if ((character >= '0' && character <= '9') ||
             (character >= 'a' && character <= 'z') ||
             (character >= 'A' && character <= 'Z') ||
             (character >= 0x80 && character <= 0x9a) ||
             (character >= 0xa0 && character <= 0xa7) ||
             character == 0xe1) accepted = 1;

    if (accepted) pharaoh_text_input_insert(state, character);
    pharaoh_text_input_update_cursor(state);
}

/* Complete two-callee C translation of Pharaoh.exe 0x004CC4C0. */
void pharaoh_keyboard_delete(PharaohTextInputState *state)
{
    const int32_t field_id = state->active_field;
    if (field_id > 0) {
        char *const text = state->fields[field_id].text_buffer;
        pharaoh_text_truncate_at_pointer(
            (unsigned char *)(text + state->cursor_positions[field_id]),
            (unsigned char *)(text + state->text_lengths[field_id]));
        pharaoh_text_input_update_cursor(state);
    }
}

/* Native 0x004CC350 is a direct thunk to 0x004CC4C0. */
void pharaoh_keyboard_delete_thunk(PharaohTextInputState *state)
{
    pharaoh_keyboard_delete(state);
}

/* Complete one-callee C translation of Pharaoh.exe 0x004CC330. */
void pharaoh_keyboard_backspace(PharaohTextInputState *state)
{
    int32_t *const cursor = &state->cursor_positions[state->active_field];
    if (*cursor > 0) {
        --*cursor;
        pharaoh_keyboard_delete(state);
    }
}
