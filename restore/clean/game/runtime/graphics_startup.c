#include "pharaoh/runtime/graphics_startup.h"

#include <stddef.h>

static const char graphics_begin_error[] = "ERR:gfxBegin failed.";
static const char graphics_device_released[] = "OK :DD Released.";

static void release_graphics_interface(
    const PharaohGraphicsStartupOperations *operations,
    void **interface_pointer)
{
    if (*interface_pointer != NULL) {
        (void)operations->release_graphics_interface(operations->user_data,
            *interface_pointer);
        *interface_pointer = NULL;
    }
}

int32_t pharaoh_graphics_resources_release(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations,
    int32_t release_device)
{
    release_graphics_interface(operations, &state->graphics_clipper);
    release_graphics_interface(operations, &state->back_buffer_surface);
    release_graphics_interface(operations, &state->primary_surface);
    state->back_buffer_pixels = NULL;

    if (release_device != 0 && state->direct_draw_device != NULL) {
        release_graphics_interface(operations,
            &state->direct_draw_device);
        operations->report_status(operations->user_data,
            graphics_device_released, 0, 0);
    }
    return 1;
}

void pharaoh_graphics_session_release(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations,
    int32_t release_device)
{
    if (state->graphics_interface != NULL) {
        (void)operations->release_graphics_interface(operations->user_data,
            state->graphics_interface);
        state->graphics_interface = NULL;
    }

    if (state->display_session_active != 0) {
        (void)pharaoh_graphics_resources_release(state, operations,
            release_device);
        state->display_session_active = 0;
    }
}

int32_t pharaoh_graphics_session_begin(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations)
{
    state->display_initialization_in_progress = UINT32_C(1);
    pharaoh_graphics_session_release(state, operations, 0);
    state->display_session_active =
        pharaoh_graphics_device_initialize(state, operations);

    if (state->display_session_active == 0) {
        operations->report_status(operations->user_data,
            graphics_begin_error, 0, 0);
        return 0;
    }

    (void)operations->clear_display_surfaces(operations->user_data);
    state->display_initialization_in_progress = 0;
    state->frame_rendering_enabled = UINT32_C(1);
    state->redraw_requested = UINT8_C(1);
    return 1;
}
