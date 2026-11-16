#include "pharaoh/runtime/application_display_startup.h"

enum {
    DESKTOP_METRIC_WIDTH = 0,
    DESKTOP_METRIC_HEIGHT = 1
};

int32_t pharaoh_application_display_startup(
    PharaohApplicationDisplayStartup *startup, int32_t settings_display_mode)
{
    PharaohGraphicsStartupState *state = startup->graphics;
    const PharaohGraphicsStartupOperations *operations =
        startup->graphics_operations;

    pharaoh_initial_window_geometry(settings_display_mode,
        &startup->geometry);
    state->display_mode_selection = startup->geometry.display_mode;
    state->screen_width = startup->geometry.viewport_width;
    state->screen_height = startup->geometry.viewport_height;
    state->render_width = startup->geometry.viewport_width;
    state->render_height = startup->geometry.viewport_height;
    state->mode_status_byte = startup->geometry.display_mode_status;
    state->desktop_width = operations->get_system_metric(
        operations->user_data, DESKTOP_METRIC_WIDTH);
    state->desktop_height = operations->get_system_metric(
        operations->user_data, DESKTOP_METRIC_HEIGHT);
    return pharaoh_graphics_session_begin(state, operations);
}
