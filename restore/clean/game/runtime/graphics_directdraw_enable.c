#include "pharaoh/runtime/graphics_startup.h"

enum {
    PHARAOH_GRAPHICS_CAPABILITY_NUM_COLORS = 0x18,
    PHARAOH_GRAPHICS_CAPABILITY_PLANES = 0x0e,
    PHARAOH_GRAPHICS_CAPABILITY_BITS_PER_PIXEL = 0x0c,
    PHARAOH_GRAPHICS_METRIC_SCREEN_WIDTH = 0,
    PHARAOH_GRAPHICS_METRIC_SCREEN_HEIGHT = 1,
    PHARAOH_GRAPHICS_PALETTE_ENTRY_COUNT = 256,
    PHARAOH_GRAPHICS_WINDOW_STYLE_INDEX = -16,
    PHARAOH_GRAPHICS_WINDOW_EXTENDED_STYLE_INDEX = -20,
    PHARAOH_GRAPHICS_WINDOWED_COOPERATIVE_FLAGS = 0x08,
    PHARAOH_GRAPHICS_FULLSCREEN_COOPERATIVE_FLAGS = 0x13,
    PHARAOH_GRAPHICS_WINDOWED_STYLE_KEEP_MASK = 0x7f30ffff,
    PHARAOH_GRAPHICS_WINDOWED_STYLE_BITS = 0x00cf0000,
    PHARAOH_GRAPHICS_FULLSCREEN_STYLE_KEEP_MASK = 0xff3bffff,
    PHARAOH_GRAPHICS_FULLSCREEN_STYLE_BITS = 0x90000000,
    PHARAOH_GRAPHICS_FULLSCREEN_BITS_PER_PIXEL = 16,
    PHARAOH_GRAPHICS_256_COLOR_MESSAGE_ID = 0x39,
    PHARAOH_WINDOW_Z_ORDER_TOP = 0,
    PHARAOH_WINDOW_Z_ORDER_NOT_TOPMOST = -2,
    PHARAOH_WINDOW_POSITION_NO_SIZE = 0x01,
    PHARAOH_WINDOW_POSITION_NO_MOVE = 0x02,
    PHARAOH_WINDOW_POSITION_NO_Z_ORDER = 0x04,
    PHARAOH_WINDOW_POSITION_NO_ACTIVATE = 0x10,
    PHARAOH_WINDOWED_SIZE_FLAGS =
        PHARAOH_WINDOW_POSITION_NO_ACTIVATE |
        PHARAOH_WINDOW_POSITION_NO_MOVE |
        PHARAOH_WINDOW_POSITION_NO_Z_ORDER,
    PHARAOH_WINDOWED_Z_ORDER_FLAGS =
        PHARAOH_WINDOW_POSITION_NO_ACTIVATE |
        PHARAOH_WINDOW_POSITION_NO_MOVE |
        PHARAOH_WINDOW_POSITION_NO_SIZE,
    PHARAOH_WINDOWED_CENTER_FLAGS =
        PHARAOH_WINDOW_POSITION_NO_ACTIVATE |
        PHARAOH_WINDOW_POSITION_NO_SIZE |
        PHARAOH_WINDOW_POSITION_NO_Z_ORDER
};

static const char direct_draw_create_failed[] =
    "ERR:DD Enable error level:";
static const char cooperative_level_failed[] =
    "ERR:DD Not setCooperative Level :";
static const char cooperative_level_set[] = "OK :DD SetCooperative Level.";
static const char switching_to_fullscreen[] =
    "OK :DD 256 cols switching to full screen.";
static const char display_mode_failed[] = "ERR:DD SetMode failed err :";
static const char windowed_resolution_kept[] = "OK :DD Kept resolution.";
static const char fullscreen_resolution_set[] = "OK :DD Set resolution.";

static void set_window_style(
    const PharaohGraphicsStartupOperations *operations, void *window,
    uint32_t keep_mask, uint32_t style_bits)
{
    const uint32_t style = (uint32_t)operations->get_window_long(
        operations->user_data, window, PHARAOH_GRAPHICS_WINDOW_STYLE_INDEX);
    operations->set_window_long(operations->user_data, window,
        PHARAOH_GRAPHICS_WINDOW_STYLE_INDEX,
        (style & keep_mask) | style_bits);
}

static int32_t set_fullscreen_display_mode(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations,
    void *direct_draw_device)
{
    int32_t result = operations->set_display_mode(operations->user_data,
        direct_draw_device, state->render_width, state->render_height,
        PHARAOH_GRAPHICS_FULLSCREEN_BITS_PER_PIXEL);

    if (result != 0) {
        operations->prepare_display_mode_retry(operations->user_data, 2);
        operations->resize_graphics_screen(operations->user_data);
        operations->clear_city_warnings(operations->user_data);
        result = operations->set_display_mode(operations->user_data,
            direct_draw_device, state->render_width, state->render_height,
            PHARAOH_GRAPHICS_FULLSCREEN_BITS_PER_PIXEL);
        if (result != 0) {
            operations->prepare_display_mode_retry(operations->user_data, 1);
            operations->resize_graphics_screen(operations->user_data);
            operations->clear_city_warnings(operations->user_data);
            result = operations->set_display_mode(operations->user_data,
                direct_draw_device, state->render_width, state->render_height,
                PHARAOH_GRAPHICS_FULLSCREEN_BITS_PER_PIXEL);
        }
    }

    return result;
}

static void configure_windowed_mode(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations)
{
    PharaohGraphicsWindowRect window_rect;
    PharaohGraphicsWindowRect work_area = { 0, 0, 0, 0 };
    void *const window = state->main_window;
    void *menu;
    uint32_t extended_style;
    uint32_t style;
    int32_t has_menu;
    int32_t width;
    int32_t height;

    set_window_style(operations, window, PHARAOH_GRAPHICS_WINDOWED_STYLE_KEEP_MASK,
        PHARAOH_GRAPHICS_WINDOWED_STYLE_BITS);

    window_rect.left = 0;
    window_rect.top = 0;
    window_rect.right = (int32_t)state->render_width;
    window_rect.bottom = (int32_t)state->render_height;
    extended_style = (uint32_t)operations->get_window_long(
        operations->user_data, window,
        PHARAOH_GRAPHICS_WINDOW_EXTENDED_STYLE_INDEX);
    menu = operations->get_window_menu(operations->user_data, window);
    style = (uint32_t)operations->get_window_long(operations->user_data,
        window, PHARAOH_GRAPHICS_WINDOW_STYLE_INDEX);
    has_menu = (int32_t)(menu != NULL);
    (void)operations->adjust_window_rect(operations->user_data, &window_rect,
        style, has_menu, extended_style);

    state->window_offset_x = (state->desktop_width -
        (int32_t)state->render_width) / 2;
    state->window_offset_y = (state->desktop_height -
        (int32_t)state->render_height) / 2;
    width = (int32_t)((uint32_t)window_rect.right -
        (uint32_t)window_rect.left);
    height = (int32_t)((uint32_t)window_rect.bottom -
        (uint32_t)window_rect.top);

    (void)operations->set_window_position(operations->user_data, window,
        PHARAOH_WINDOW_Z_ORDER_TOP, 0, 0, width, height,
        PHARAOH_WINDOWED_SIZE_FLAGS);
    (void)operations->set_window_position(operations->user_data, window,
        PHARAOH_WINDOW_Z_ORDER_NOT_TOPMOST, 0, 0, 0, 0,
        PHARAOH_WINDOWED_Z_ORDER_FLAGS);
    (void)operations->get_work_area(operations->user_data, &work_area);
    (void)operations->get_window_rect(operations->user_data, window,
        &window_rect);

    /* Native code clamps this local rectangle, then positions from the saved
       center coordinates instead of using the clamped left/top values. */
    if (window_rect.left < work_area.left) {
        window_rect.left = work_area.left;
    }
    if (window_rect.top < work_area.top) {
        window_rect.top = work_area.top;
    }
    (void)operations->set_window_position(operations->user_data, window,
        PHARAOH_WINDOW_Z_ORDER_TOP, state->window_offset_x,
        state->window_offset_y, 0, 0, PHARAOH_WINDOWED_CENTER_FLAGS);
}

int32_t pharaoh_graphics_enable_direct_draw(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations)
{
    void *device_context = operations->get_device_context(
        operations->user_data, NULL);
    void *direct_draw_device;
    int32_t planes;
    int32_t bits_per_plane;
    int32_t result;
    int32_t created_device;

    (void)operations->get_device_capability(operations->user_data,
        device_context, PHARAOH_GRAPHICS_CAPABILITY_NUM_COLORS);
    planes = operations->get_device_capability(operations->user_data,
        device_context, PHARAOH_GRAPHICS_CAPABILITY_PLANES);
    bits_per_plane = operations->get_device_capability(operations->user_data,
        device_context, PHARAOH_GRAPHICS_CAPABILITY_BITS_PER_PIXEL);
    state->desktop_color_depth = (uint32_t)planes * (uint32_t)bits_per_plane;
    operations->release_device_context(operations->user_data, NULL,
        device_context);

    state->desktop_width = operations->get_system_metric(
        operations->user_data, PHARAOH_GRAPHICS_METRIC_SCREEN_WIDTH);
    state->desktop_height = operations->get_system_metric(
        operations->user_data, PHARAOH_GRAPHICS_METRIC_SCREEN_HEIGHT);

    if (state->desktop_width < state->render_width) {
        if (state->color_mode_selection == 0) {
            operations->dispatch_message(operations->user_data,
                PHARAOH_GRAPHICS_256_COLOR_MESSAGE_ID, 0, 0);
        }
        state->color_mode_selection = 1;
    } else if (state->color_mode_selection == 0 &&
        state->desktop_color_depth != 16) {
        operations->dispatch_message(operations->user_data,
            PHARAOH_GRAPHICS_256_COLOR_MESSAGE_ID, 0, 0);
        operations->report_status(operations->user_data,
            switching_to_fullscreen, 0, 0);
        state->color_mode_selection = 1;
    }

    if (state->color_mode_selection != 0) {
        (void)operations->get_palette_entries(operations->user_data, NULL,
            0, PHARAOH_GRAPHICS_PALETTE_ENTRY_COUNT, state->palette);
    }

    created_device = (int32_t)(state->direct_draw_device == NULL);
    if (created_device != 0) {
        result = operations->create_direct_draw(operations->user_data, NULL,
            &direct_draw_device, NULL);
        if (result != 0) {
            operations->report_status(operations->user_data,
                direct_draw_create_failed, 0, result);
            return 0;
        }
    } else {
        direct_draw_device = state->direct_draw_device;
    }

    if (created_device != 0) {
        const uint32_t cooperative_flags = state->color_mode_selection == 0
            ? PHARAOH_GRAPHICS_WINDOWED_COOPERATIVE_FLAGS
            : PHARAOH_GRAPHICS_FULLSCREEN_COOPERATIVE_FLAGS;
        result = operations->set_cooperative_level(operations->user_data,
            direct_draw_device, state->main_window, cooperative_flags);
        if (result != 0) {
            operations->report_status(operations->user_data,
                cooperative_level_failed, 0, result);
            return 0;
        }
        operations->report_status(operations->user_data,
            cooperative_level_set, 0, 0);
    }

    if (state->color_mode_selection == 0) {
        configure_windowed_mode(state, operations);
        operations->report_status(operations->user_data,
            windowed_resolution_kept, 0, 0);
    } else {
        set_window_style(operations, state->main_window,
            (uint32_t)PHARAOH_GRAPHICS_FULLSCREEN_STYLE_KEEP_MASK,
            (uint32_t)PHARAOH_GRAPHICS_FULLSCREEN_STYLE_BITS);
        result = set_fullscreen_display_mode(state, operations,
            direct_draw_device);
        if (result != 0) {
            operations->report_status(operations->user_data,
                display_mode_failed, 0, result);
            return 0;
        }
        operations->report_status(operations->user_data,
            fullscreen_resolution_set, 0, 0);
    }

    state->direct_draw_device = direct_draw_device;
    return 1;
}
