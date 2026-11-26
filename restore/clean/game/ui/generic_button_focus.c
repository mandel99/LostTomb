#include "pharaoh/ui/generic_button_focus.h"

#include <stdint.h>
#include <stddef.h>

_Static_assert(sizeof(PharaohImageButtonRecord) == 0x24 &&
    offsetof(PharaohImageButtonRecord, trigger) == 8 &&
    offsetof(PharaohImageButtonRecord, image_group) == 0x0a &&
    offsetof(PharaohImageButtonRecord, image_offset) == 0x0c &&
    offsetof(PharaohImageButtonRecord, enabled) == 0x18 &&
    offsetof(PharaohImageButtonRecord, activation_ticks) == 0x19 &&
    offsetof(PharaohImageButtonRecord, pressed) == 0x1a &&
    offsetof(PharaohImageButtonRecord, animation) == 0x1b,
    "native image-button record layout");
_Static_assert(sizeof(PharaohSquareButtonRecord) == 0x18,
    "native square-button record layout");
_Static_assert(sizeof(PharaohMenuBarFocusRecord) == 0x10,
    "native menu-bar focus record layout");

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

int32_t pharaoh_generic_buttons_get_focus(
    int32_t origin_x, int32_t origin_y,
    const PharaohMenuRect *rectangles, int32_t rectangle_count,
    int32_t cursor_x, int32_t cursor_y,
    PharaohGenericButtonFocusState *focus_state)
{
    int32_t index;

    for (index = 0; index < rectangle_count; ++index) {
        const PharaohMenuRect *const rectangle = &rectangles[index];
        const int32_t left = wrap_add(origin_x, rectangle->left);
        const int32_t right = wrap_add(origin_x, rectangle->right);
        const int32_t top = wrap_add(origin_y, rectangle->top);
        const int32_t bottom = wrap_add(origin_y, rectangle->bottom);

        if (left <= cursor_x && cursor_x < right &&
            top <= cursor_y && cursor_y < bottom) {
            focus_state->hit_mouse_y = cursor_y;
            focus_state->hit_mouse_x = cursor_x;
            return index + 1;
        }
    }

    return 0;
}

/* Complete C translation of Pharaoh.exe 0x00425B50. */
void pharaoh_image_buttons_clear_radio(
    PharaohImageButtonRecord *buttons, int32_t button_count,
    uint8_t *redraw_requested)
{
    int32_t index;
    for (index = 0; index < button_count; ++index) {
        if (buttons[index].activation_ticks != 0 &&
            buttons[index].trigger == 2) {
            buttons[index].activation_ticks = 0;
            *redraw_requested = 1;
        }
    }
}

/* Complete C translation of Pharaoh.exe 0x00425B80. */
void pharaoh_image_buttons_decay(
    PharaohImageButtonRecord *buttons, int32_t button_count,
    uint8_t *redraw_requested)
{
    int32_t index;
    for (index = 0; index < button_count; ++index) {
        PharaohImageButtonRecord *button = &buttons[index];
        if (button->activation_ticks != 0 &&
            (button->trigger == 4 || button->trigger == 6) &&
            --button->activation_ticks == 0) {
            button->activation_ticks = 0;
            *redraw_requested = 1;
        }
    }
}

/* Complete one-callee C translation of Pharaoh.exe 0x00425730. */
void pharaoh_image_buttons_draw(int32_t origin_x, int32_t origin_y,
    PharaohImageButtonRecord *buttons, int32_t button_count,
    PharaohImageButtonDrawState *state)
{
    for (int32_t index = 0; index < button_count; ++index) {
        PharaohImageButtonRecord *const button = &buttons[index];
        uint32_t variant;
        *state->draw_x = wrap_add(origin_x, button->x);
        *state->draw_y = wrap_add(origin_y, button->y);
        *state->active_sprite_image =
            state->sprite_group_bases[button->image_group * 2] +
            button->image_offset;
        if (*state->ui_redraw == 0 &&
            *state->window_redraw_requested == 0 &&
            button->animation == 0) {
            continue;
        }
        if (button->enabled == 0) {
            *state->active_sprite_image += 3;
        } else if (button->animation == 0) {
            if (button->activation_ticks == 0) {
                if (button->pressed != 0) ++*state->active_sprite_image;
            } else {
                if (button->activation_ticks == 20) *state->force_refresh = 1;
                *state->active_sprite_image += 2;
            }
        } else {
            uint8_t phase = (uint8_t)(button->animation >> 2);
            variant = button->animation & 3u;
            if (phase == 0) {
                variant = (variant != 2u) + 1u;
                phase = 16;
            }
            button->animation = (uint8_t)(
                ((uint32_t)(phase + 63u) * 4u) | variant);
            *state->active_sprite_image += (int32_t)variant - 1;
        }
        state->draw_sprite(state->user_data, *state->active_sprite_image,
            *state->draw_x, *state->draw_y);
    }
}

/* Complete C translation of Pharaoh.exe 0x00425BC0. */
void pharaoh_image_buttons_set_tooltip(int32_t origin_x, int32_t origin_y,
    const PharaohImageButtonRecord *buttons, int32_t button_count,
    int32_t first_tooltip_id, int32_t cursor_x, int32_t cursor_y,
    PharaohButtonHitState *hit_state)
{
    int32_t index;
    for (index = 0; index < button_count; ++index) {
        const PharaohImageButtonRecord *button = &buttons[index];
        const int32_t left = wrap_add(origin_x, button->x);
        const int32_t top = wrap_add(origin_y, button->y);
        if (left <= cursor_x && cursor_x < wrap_add(left, button->width) &&
            top <= cursor_y && cursor_y < wrap_add(top, button->height)) {
            hit_state->hit_mouse_x = cursor_x;
            hit_state->hit_mouse_y = cursor_y;
            hit_state->focused_id = first_tooltip_id + index;
            return;
        }
    }
}

/* Complete C translation of Pharaoh.exe 0x00425E80. */
void pharaoh_square_buttons_get_focus(int32_t origin_x, int32_t origin_y,
    const PharaohSquareButtonRecord *buttons, int32_t button_count,
    int32_t first_button_id, int32_t cursor_x, int32_t cursor_y,
    PharaohButtonHitState *hit_state)
{
    int32_t index;
    for (index = 0; index < button_count; ++index) {
        const PharaohSquareButtonRecord *button = &buttons[index];
        const int32_t left = wrap_add(origin_x, button->x);
        const int32_t top = wrap_add(origin_y, button->y);
        if (left <= cursor_x && cursor_x < wrap_add(left, button->size) &&
            top <= cursor_y && cursor_y < wrap_add(top, button->size)) {
            hit_state->hit_mouse_x = cursor_x;
            hit_state->hit_mouse_y = cursor_y;
            hit_state->focused_id = first_button_id + index;
            return;
        }
    }
}

static void square_button_dispatch(PharaohSquareButtonInputState *state,
    const PharaohSquareButtonRecord *button)
{
    state->primary_parameter = button->primary_parameter;
    state->secondary_parameter = button->secondary_parameter;
    state->dispatch(state->user_data, button->callback_token);
}

/* Complete typed translation of Pharaoh.exe 0x00425C40. */
int32_t pharaoh_square_buttons_handle_mouse(
    int32_t origin_x, int32_t origin_y,
    PharaohSquareButtonRecord *buttons, int32_t button_count,
    PharaohSquareButtonInputState *state)
{
    uint32_t now = state->clock(state->user_data);
    const int32_t repeat_tick =
        (int32_t)(now - state->last_repeat_tick) >= 30;
    int32_t index;
    if (repeat_tick) state->last_repeat_tick = now;

    for (index = 0; index < button_count; ++index) {
        PharaohSquareButtonRecord *button = &buttons[index];
        if (button->countdown != 0) {
            --button->countdown;
            if (button->countdown == 0) {
                button->pressed = 1;
                button->repeat = 0;
                if (button->trigger == 5)
                    square_button_dispatch(state, button);
            }
        } else {
            button->repeat = 0;
        }
        if (button->trigger == 3) button->pressed = 0;
    }

    for (index = 0; index < button_count; ++index) {
        PharaohSquareButtonRecord *button = &buttons[index];
        const int32_t left = wrap_add(origin_x, button->x);
        const int32_t top = wrap_add(origin_y, button->y);
        if (left > state->cursor_x ||
            wrap_add(left, button->size) <= state->cursor_x ||
            top > state->cursor_y ||
            wrap_add(top, button->size) <= state->cursor_y) continue;

        if (state->editor_map_preview > 0 &&
            state->current_window_id == 1 && state->editor_mode == 1)
            state->focused_building = 21;

        if (button->trigger == 4) {
            if (state->left_went_down != 0 || state->left_clicked != 0) {
                button->pressed = 1;
                button->countdown = 3;
                button->repeat = 0;
                state->repeat_value = 0;
                square_button_dispatch(state, button);
                return index + 1;
            }
            if (state->left_down == 0) return 0;
            button->countdown = 3;
            if (!repeat_tick) return 0;
            ++button->repeat;
            if (button->repeat >= 48) button->repeat = 47;
            else if (button->repeat < 8 ||
                     state->repeat_schedule[button->repeat] == 0) return 0;
            state->repeat_value = button->repeat;
            square_button_dispatch(state, button);
            return index + 1;
        }
        if (button->trigger == 2) {
            button->countdown = 1;
            if (button->repeat == 0) {
                button->pressed ^= 1;
                square_button_dispatch(state, button);
            }
            return index + 1;
        }
        if (button->trigger == 5) {
            if (state->left_went_down == 0 && state->left_clicked == 0)
                return 0;
            button->countdown = 10;
            button->pressed = 1;
            return index + 1;
        }
    }
    return 0;
}

/* Complete C translation of Pharaoh.exe 0x00426100. */
int32_t pharaoh_menu_bar_focus(const PharaohMenuBarFocusRecord *entries,
    int32_t entry_count, int32_t cursor_x, int32_t cursor_y)
{
    int32_t index;
    for (index = 0; index < entry_count; ++index) {
        if (entries[index].left <= cursor_x && cursor_x < entries[index].right &&
            entries[index].top <= cursor_y &&
            cursor_y < entries[index].top + 12) return index + 1;
    }
    return 0;
}

/* Complete two-callee C translation of Pharaoh.exe 0x00504630. */
void pharaoh_building_info_draw_palace_foreground(
    int32_t building_info_x, int32_t building_info_y,
    PharaohSquareButtonRecord tax_arrow_buttons[2],
    PharaohButtonHitState *hit_state,
    PharaohSquareButtonInputState *input_state)
{
    const int32_t x = building_info_x + 226;
    const int32_t y = building_info_y - 37;
    pharaoh_square_buttons_get_focus(x, y, tax_arrow_buttons, 2, 10,
        input_state->cursor_x, input_state->cursor_y, hit_state);
    (void)pharaoh_square_buttons_handle_mouse(
        x, y, tax_arrow_buttons, 2, input_state);
}

/* Complete one-callee C translation of Pharaoh.exe 0x00515BF0. */
void pharaoh_building_info_draw_advisor_buttons(
    int32_t building_info_x, int32_t building_info_y,
    int32_t building_info_height, const int32_t shown[15],
    const uint8_t tutorial_flags[15],
    PharaohImageButtonRecord *const tutorial_buttons[15],
    PharaohImageButtonDrawState *draw_state)
{
    const uint32_t y_offset =
        (uint32_t)building_info_height * 16u - 40u;
    uint32_t x_offset = 0;
    *draw_state->ui_redraw = 1;
    for (int32_t advisor = 0; advisor < 15; ++advisor) {
        if (shown[advisor] != 0 && tutorial_flags[advisor] != 0) {
            const int32_t x = signed_dword_from_bits(
                (uint32_t)building_info_x + 40u + x_offset);
            const int32_t y = signed_dword_from_bits(
                (uint32_t)building_info_y + y_offset);
            pharaoh_image_buttons_draw(
                x, y, tutorial_buttons[advisor], 1, draw_state);
            x_offset += 27u;
        }
    }
    *draw_state->force_refresh = 0;
}
