#include "pharaoh/runtime/graphics_startup.h"

enum {
    GRAPHICS_CLEAR_SURFACE_FLAGS = 0x01000400,
    GRAPHICS_CLEAR_BACK_SURFACE_FAILED = 1,
    GRAPHICS_CLEAR_FRONT_SURFACE_FAILED = 2
};

static const char graphics_clear_failed[] =
    "ERR:DD Clearing Fill failed no:";

int32_t pharaoh_graphics_surfaces_clear(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations)
{
    int32_t back_result;
    int32_t front_result;

    operations->update_window(operations->user_data, state->main_window);
    back_result = operations->fill_surface(operations->user_data,
        state->back_buffer_surface, GRAPHICS_CLEAR_SURFACE_FLAGS, 0);
    front_result = operations->fill_surface(operations->user_data,
        state->primary_surface, GRAPHICS_CLEAR_SURFACE_FLAGS, 0);
    if (back_result == 0 && front_result == 0) {
        return 1;
    }

    operations->report_status(operations->user_data,
        graphics_clear_failed, 0,
        back_result != 0 ? GRAPHICS_CLEAR_BACK_SURFACE_FAILED
                         : GRAPHICS_CLEAR_FRONT_SURFACE_FAILED);
    return 0;
}
