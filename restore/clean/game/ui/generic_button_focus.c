#include "pharaoh/ui/generic_button_focus.h"

#include <stdint.h>

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
