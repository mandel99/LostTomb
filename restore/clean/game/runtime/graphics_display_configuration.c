#include "pharaoh/runtime/graphics_startup.h"

enum {
    DISPLAY_MODE_640X480 = 1,
    DISPLAY_MODE_800X600 = 2,
    DISPLAY_MODE_1024X768 = 3,
    DESIGN_WIDTH = 640,
    DESIGN_HEIGHT = 480,
    BYTES_PER_PIXEL = 2,
    WINDOW_STYLE_INDEX = -16,
    WINDOW_EXTENDED_STYLE_INDEX = -20,
    WINDOW_Z_ORDER_TOP = 0,
    WINDOW_Z_ORDER_NOT_TOPMOST = -2,
    WINDOW_SIZE_FLAGS = 0x16,
    WINDOW_Z_ORDER_FLAGS = 0x13
};

void pharaoh_graphics_display_size_select(
    PharaohGraphicsStartupState *state, int32_t display_mode)
{
    if (display_mode == DISPLAY_MODE_640X480) {
        state->render_width = 640;
        state->render_height = 480;
    } else if (display_mode == DISPLAY_MODE_800X600) {
        state->render_width = 800;
        state->render_height = 600;
    } else if (display_mode == DISPLAY_MODE_1024X768) {
        state->render_width = 1024;
        state->render_height = 768;
    }
}

void pharaoh_graphics_display_configure(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations,
    int32_t display_mode)
{
    PharaohGraphicsWindowRect adjusted;
    const void *const menu = operations->get_window_menu(
        operations->user_data, state->main_window);
    const uint32_t extended_style = (uint32_t)operations->get_window_long(
        operations->user_data, state->main_window,
        WINDOW_EXTENDED_STYLE_INDEX);
    const uint32_t style = (uint32_t)operations->get_window_long(
        operations->user_data, state->main_window, WINDOW_STYLE_INDEX);

    pharaoh_graphics_display_size_select(state, display_mode);
    state->display_mode_selection = display_mode;
    state->screen_width = state->render_width;
    state->screen_height = state->render_height;
    state->hit_width = state->render_width;
    state->hit_height = state->render_height;
    state->ui_origin_x = (state->render_width - DESIGN_WIDTH) / 2;
    state->ui_origin_y = (state->render_height - DESIGN_HEIGHT) / 2;
    state->rgb565_pitch_bytes = state->render_width * BYTES_PER_PIXEL;
    state->framebuffer_bytes = (uint32_t)state->rgb565_pitch_bytes *
        (uint32_t)state->render_height;
    state->viewport_height = state->render_height;
    state->display_rectangle.left = 0;
    state->display_rectangle.top = 0;
    state->display_rectangle.right = state->render_width;
    state->display_rectangle.bottom = state->render_height;

    adjusted = state->display_rectangle;
    (void)operations->adjust_window_rect(operations->user_data, &adjusted,
        style, menu != NULL, extended_style);
    state->window_offset_x = (state->desktop_width - state->render_width) / 2;
    state->window_offset_y = (state->desktop_height - state->render_height) / 2;

    (void)operations->set_window_position(operations->user_data,
        state->main_window, WINDOW_Z_ORDER_TOP, 0, 0,
        adjusted.right - adjusted.left, adjusted.bottom - adjusted.top,
        WINDOW_SIZE_FLAGS);
    (void)operations->set_window_position(operations->user_data,
        state->main_window, WINDOW_Z_ORDER_NOT_TOPMOST, 0, 0, 0, 0,
        WINDOW_Z_ORDER_FLAGS);
    operations->update_window(operations->user_data, state->main_window);
    operations->set_focus(operations->user_data, state->main_window);
}
