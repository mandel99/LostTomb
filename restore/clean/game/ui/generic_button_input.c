#include "pharaoh/ui/generic_button_input.h"

#include <stdint.h>

enum { CLEAR_DRAG_ACTION_ON_TRIGGERED_HOLD = 0 };

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

static int button_contains_cursor(const PharaohGenericButtonInputState *state,
                                  const PharaohGenericButton *button)
{
    const int32_t left = wrap_add(state->origin_x, button->bounds.left);
    const int32_t right = wrap_add(state->origin_x, button->bounds.right);
    const int32_t top = wrap_add(state->origin_y, button->bounds.top);
    const int32_t bottom = wrap_add(state->origin_y, button->bounds.bottom);

    return left <= state->cursor_x && state->cursor_x < right &&
        top <= state->cursor_y && state->cursor_y < bottom;
}

static void activate_button(PharaohGenericButtonInputState *state,
                            PharaohGenericButton *button)
{
    button->activated = 1;
    state->selected_primary_parameter = button->primary_parameter;
    state->selected_secondary_parameter = button->secondary_parameter;
}

int32_t pharaoh_generic_buttons_handle_mouse(
    PharaohGenericButtonInputState *state,
    PharaohGenericButton *buttons,
    int32_t button_count)
{
    int32_t index;

    for (index = 0; index < button_count; ++index) {
        PharaohGenericButton *const button = &buttons[index];
        int invoke_left = 0;
        int invoke_right = 0;

        if (!button_contains_cursor(state, button)) {
            continue;
        }

        switch (button->trigger) {
        case PHARAOH_GENERIC_BUTTON_TRIGGER_PRESS:
            if (state->left_pressed != 0 || state->left_double_click != 0) {
                invoke_left = 1;
            } else if (state->right_pressed != 0) {
                invoke_right = 1;
            } else {
                continue;
            }
            break;

        case PHARAOH_GENERIC_BUTTON_TRIGGER_RELEASE:
            if (state->left_released != 0) {
                invoke_left = 1;
            } else if (state->right_released != 0) {
                invoke_right = 1;
            } else {
                continue;
            }
            break;

        case PHARAOH_GENERIC_BUTTON_TRIGGER_LEFT_PRESS:
            if (state->left_pressed == 0 && state->left_double_click == 0) {
                continue;
            }
            invoke_left = 1;
            break;

        case PHARAOH_GENERIC_BUTTON_TRIGGER_HOLD:
            if (state->left_down == 0) {
                if (state->left_pressed != 0 || state->left_double_click != 0) {
                    state->clear_drag_action =
                        CLEAR_DRAG_ACTION_ON_TRIGGERED_HOLD;
                } else if (state->right_down == 0) {
                    if (state->right_pressed == 0 &&
                        state->right_double_click == 0) {
                        continue;
                    }
                    state->clear_drag_action =
                        CLEAR_DRAG_ACTION_ON_TRIGGERED_HOLD;
                } else if (state->right_pressed != 0 ||
                           state->right_double_click != 0) {
                    state->clear_drag_action =
                        CLEAR_DRAG_ACTION_ON_TRIGGERED_HOLD;
                } else if (state->hold_action_enabled == 0) {
                    continue;
                }
            } else if (state->left_pressed != 0 ||
                       state->left_double_click != 0 ||
                       state->right_pressed != 0 ||
                       state->right_double_click != 0) {
                state->clear_drag_action =
                    CLEAR_DRAG_ACTION_ON_TRIGGERED_HOLD;
            } else if (state->hold_action_enabled == 0) {
                continue;
            }

            if (state->left_down != 0) {
                invoke_left = 1;
            } else if (state->right_down != 0) {
                invoke_right = 1;
            }
            break;

        default:
            continue;
        }

        activate_button(state, button);
        if (invoke_left != 0) {
            button->left_action(state->action_context);
        } else if (invoke_right != 0) {
            button->right_action(state->action_context);
        }
        return index + 1;
    }

    return 0;
}
