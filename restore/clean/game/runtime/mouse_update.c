#include "pharaoh/runtime/mouse_update.h"

#include "pharaoh/core/percentage_adjust.h"
#include "pharaoh/core/percentage_scale.h"

#include <stdint.h>

enum {
    MOUSE_DRAG_ANCHOR_STATE = 1,
    MOUSE_DRAGGING_STATE = 2,
    MOUSE_DRAG_THRESHOLD = 0x32,
    MOUSE_LEFT_BUTTON_MASK = 1,
    MOUSE_RIGHT_BUTTON_MASK = 2,
    MOUSE_LEFT_SOUND_EFFECT_SLOT = 0
};

static int32_t signed_dword_from_bits(uint32_t bits)
{
    if ((bits & UINT32_C(0x80000000)) != 0) {
        return (int32_t)((int64_t)bits - INT64_C(0x100000000));
    }
    return (int32_t)bits;
}

static int32_t wrap_add(int32_t left, int32_t right)
{
    return signed_dword_from_bits((uint32_t)left + (uint32_t)right);
}

static int32_t wrap_subtract(int32_t left, int32_t right)
{
    return signed_dword_from_bits((uint32_t)left - (uint32_t)right);
}

static int16_t wrap_to_signed_word(int32_t value)
{
    const uint16_t bits = (uint16_t)(uint32_t)value;
    if (bits > INT16_MAX) {
        return (int16_t)((int32_t)bits - INT32_C(0x10000));
    }
    return (int16_t)bits;
}

PharaohMouseUpdateStatus pharaoh_mouse_update(
    PharaohMouseUpdateState *state,
    const PharaohMouseUpdateOperations *operations,
    PharaohSoundEffectRequestState *sound_state,
    const PharaohSoundEffectRequestOperations *sound_operations)
{
    PharaohMousePoint cursor_position;

    operations->query_cursor_position(operations->context, &cursor_position);
    state->cursor_screen_x = cursor_position.x;
    state->cursor_screen_y = cursor_position.y;
    if (state->cursor_sample_enabled != 0) {
        state->cursor_sample_x = wrap_to_signed_word(cursor_position.x);
        state->cursor_sample_y = wrap_to_signed_word(cursor_position.y);
    }

    if (state->drag_state == MOUSE_DRAG_ANCHOR_STATE) {
        if (wrap_add(state->drag_anchor_x, MOUSE_DRAG_THRESHOLD) <
            state->logical_x) {
            state->drag_state = MOUSE_DRAGGING_STATE;
        }
        if (state->logical_x <
            wrap_subtract(state->drag_anchor_x, MOUSE_DRAG_THRESHOLD)) {
            state->drag_state = MOUSE_DRAGGING_STATE;
        }
        if (wrap_add(state->drag_anchor_y, MOUSE_DRAG_THRESHOLD) <
            state->logical_y) {
            state->drag_state = MOUSE_DRAGGING_STATE;
        }
        if (state->logical_y <
            wrap_subtract(state->drag_anchor_y, MOUSE_DRAG_THRESHOLD)) {
            state->drag_state = MOUSE_DRAGGING_STATE;
        }
    }

    state->previous_logical_x = state->logical_x;
    state->previous_right_button_down = state->right_button_down;
    state->previous_logical_y = state->logical_y;
    state->previous_left_button_down = state->left_button_down;
    state->pointer_changed = 0;

    const int32_t viewport_width =
        wrap_subtract(state->viewport_right, state->viewport_left);
    const int32_t viewport_height =
        wrap_subtract(state->viewport_bottom, state->viewport_top);
    const PharaohPercentageScaleResult x_scale = pharaoh_scale_value_to_percent(
        state->cursor_scale_x, viewport_width);
    if (x_scale.status != PHARAOH_PERCENTAGE_SCALE_VALUE) {
        return PHARAOH_MOUSE_UPDATE_SIGNED_DIVIDE_OVERFLOW;
    }
    const PharaohPercentageScaleResult y_scale = pharaoh_scale_value_to_percent(
        state->cursor_scale_y, viewport_height);
    if (y_scale.status != PHARAOH_PERCENTAGE_SCALE_VALUE) {
        return PHARAOH_MOUSE_UPDATE_SIGNED_DIVIDE_OVERFLOW;
    }

    state->logical_x = pharaoh_adjust_by_percentage(
        (int32_t)state->cursor_sample_x, x_scale.value);
    state->logical_y = pharaoh_adjust_by_percentage(
        (int32_t)state->cursor_sample_y, y_scale.value);

    state->right_button_down =
        (state->raw_button_bits & MOUSE_RIGHT_BUTTON_MASK) != 0;
    state->button_state_changed = 0;
    state->left_button_down = 0;
    state->left_button_pressed = 0;
    state->left_button_released = 0;
    state->right_button_pressed = 0;
    state->right_button_released = 0;
    if ((state->raw_button_bits & MOUSE_LEFT_BUTTON_MASK) != 0) {
        state->left_button_down = 1;
    }

    if (state->previous_logical_x != state->logical_x) {
        state->pointer_changed = 1;
    }
    if (state->previous_logical_y == state->logical_y) {
        if (state->pointer_changed == 0) {
            state->time_since_pointer_change =
                state->current_time - state->last_pointer_change_time;
        }
    } else {
        state->pointer_changed = 1;
    }
    if (state->pointer_changed != 0) {
        state->last_pointer_change_time = state->current_time;
        state->time_since_pointer_change = 0;
    }

    if (state->left_button_down != state->previous_left_button_down) {
        state->pointer_changed = 1;
        state->button_state_changed = 1;
        if (state->left_button_down != 0) {
            state->left_button_pressed = state->left_button_down;
            pharaoh_sound_effect_request(
                sound_state, sound_operations, MOUSE_LEFT_SOUND_EFFECT_SLOT);
        } else {
            state->left_button_released = 1;
        }
    }

    if (state->right_button_down != state->previous_right_button_down) {
        state->pointer_changed = 1;
        state->button_state_changed = 1;
        if (state->right_button_down != 0) {
            state->right_button_auxiliary = 0;
            state->right_button_pressed = state->right_button_down;
            return PHARAOH_MOUSE_UPDATE_COMPLETE;
        }
        state->right_button_released = 1;
    }

    state->right_button_auxiliary = 0;
    return PHARAOH_MOUSE_UPDATE_COMPLETE;
}
