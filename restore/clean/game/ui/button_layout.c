#include "pharaoh/ui/button_layout.h"

#include "pharaoh/ui/window_layout_key.h"

#include <stdint.h>

enum {
    PHARAOH_BUTTON_LAYOUT_CLIP_SIZE = 6,
    PHARAOH_BUTTON_LAYOUT_HIGHLIGHT_MODE = 2,
    PHARAOH_BUTTON_LAYOUT_STYLE = 0,
    PHARAOH_BUTTON_LAYOUT_COLOR = 0xffff
};

static int32_t wrap_add(int32_t left, int32_t right)
{
    const uint32_t result_bits = (uint32_t)left + (uint32_t)right;
    if ((result_bits & UINT32_C(0x80000000)) != 0) {
        return (int32_t)((int64_t)result_bits - INT64_C(0x100000000));
    }
    return (int32_t)result_bits;
}

/* Complete hot-C translation of native 0x004D6090. */
void pharaoh_ui_layout_highlight(PharaohLayoutHighlightState *state,
    int32_t kind, int32_t x, int32_t y, int32_t width, int32_t height,
    int32_t base_x, int32_t base_y, int32_t offset_x, int32_t offset_y)
{
    const int32_t screen_x = wrap_add(base_x, offset_x);
    const int32_t screen_y = wrap_add(base_y, offset_y);
    if (kind != state->edit_mode || screen_x >= state->mouse_x ||
        state->mouse_x >= wrap_add(screen_x, width) ||
        screen_y >= state->mouse_y ||
        state->mouse_y >= wrap_add(screen_y, height)) return;
    state->clear_highlight(state->user_data);
    state->marked_x = base_x;
    state->marked_key_x = (int32_t)((uint32_t)x - (uint32_t)offset_x);
    state->marked_key_y = (int32_t)((uint32_t)y - (uint32_t)offset_y);
    state->marked_y = base_y;
    state->marked_width = width;
    state->marked_height = height;
    state->draw_border(state->user_data, 1, screen_x, screen_y,
        width, height, 0xffff);
    state->draw_border(state->user_data, 0, screen_x, screen_y,
        width, height, 0xffff);
}

int32_t pharaoh_ui_layout_buttons(PharaohButtonLayoutState *state,
    const PharaohButtonLayoutOperations *operations,
    int32_t origin_x, int32_t origin_y,
    PharaohGenericButton *buttons, int32_t button_count,
    int32_t button_index)
{
    const int32_t layout_key = pharaoh_ui_layout_window_key(
        state->window_key, state->window_seven_layout_base);
    PharaohMenuLayoutSlot *const slot = &state->slots_by_key[layout_key];

    (void)button_count;
    if (slot->initialized == 0) {
        pharaoh_menu_buttons_initialize((PharaohGenericButton *)buttons,
            slot->node);
        slot->initialized = 1;
        return 0;
    }
    if (button_index == 0) {
        return 0;
    }

    const PharaohGenericButton *const button = &buttons[button_index - 1];
    if (state->highlight_mode == PHARAOH_BUTTON_LAYOUT_HIGHLIGHT_MODE) {
        const int32_t left = button->bounds.left;
        const int32_t top = button->bounds.top;
        const int32_t width =
            (int32_t)button->bounds.right - button->bounds.left;
        const int32_t height =
            (int32_t)button->bounds.bottom - button->bounds.top;
        const int32_t highlight_x = wrap_add(origin_x, left);
        const int32_t highlight_y = wrap_add(origin_y, top);

        operations->set_clip_rectangle(operations->context, 0, 0,
            PHARAOH_BUTTON_LAYOUT_CLIP_SIZE,
            PHARAOH_BUTTON_LAYOUT_CLIP_SIZE);
        state->selected_button = button;
        state->selected_button_index = button_index;
        state->selected_left = left;
        state->selected_top = top;
        state->selected_width = width;
        state->selected_height = height;
        state->highlighted_button_index = button_index;
        state->highlight_auxiliary_state = 0;

        operations->highlight_actor_panel(operations->context, highlight_x);
        operations->draw_highlight_rectangle(operations->context,
            PHARAOH_BUTTON_LAYOUT_STYLE, highlight_x, highlight_y,
            width, height, PHARAOH_BUTTON_LAYOUT_COLOR);
    }

    return button_index;
}
