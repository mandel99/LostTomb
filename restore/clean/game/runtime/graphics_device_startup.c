#include "pharaoh/runtime/graphics_startup.h"

#include <stdio.h>

static const char direct_draw_disabled[] =
    "ERR:GFX DirectDraw not enabled.";
static const char direct_draw_enabled[] = "OK :GFX DirectDraw enabled.";
static const char flipping_surfaces_failed[] =
    "ERR:GFX couldn't create flipping surface.";
static const char fallback_mode_attempt[] =
    "ERR:Trying default display mode";
static const char old_display_format[] =
    "Old res=%d,%d; Color mode=%d";
static const char front_buffer_format[] = "Front Buffer: %x\n";
static const char back_buffer_format[] = "Back Buffer:  %x\n";

int32_t pharaoh_graphics_device_initialize(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations)
{
    char old_display_message[100];
    int32_t result = pharaoh_graphics_enable_direct_draw(state, operations);

    if (result != 1) {
        operations->report_status(operations->user_data,
            direct_draw_disabled, 0, 0);
        return 0;
    }
    operations->report_status(operations->user_data,
        direct_draw_enabled, 0, 0);

    result = pharaoh_graphics_surfaces_create(state, operations);
    if (result != 1) {
        operations->report_status(operations->user_data,
            flipping_surfaces_failed, 0, 0);
        operations->report_status(operations->user_data,
            fallback_mode_attempt, 0, 0);
        (void)pharaoh_graphics_resources_release(state, operations, 1);
        state->display_mode_selection = state->previous_display_mode;
        state->color_mode_selection = state->previous_color_mode;
        pharaoh_graphics_display_configure(state, operations,
            state->previous_display_mode);
        (void)snprintf(old_display_message, sizeof(old_display_message),
            old_display_format, state->screen_width, state->screen_height,
            state->color_mode_code);
        operations->report_status(operations->user_data,
            old_display_message, 0, 0);

        result = pharaoh_graphics_enable_direct_draw(state, operations);
        if (result == 1) {
            result = pharaoh_graphics_surfaces_create(state, operations);
        }
        if (result != 1) {
            (void)pharaoh_graphics_resources_release(state, operations, 1);
            return 0;
        }
    }

    (void)pharaoh_graphics_surface_context_initialize(&state->surface_context,
        state->primary_surface, state->back_buffer_surface, operations);
    operations->output_surface_debug(operations->user_data,
        front_buffer_format, state->primary_surface);
    operations->output_surface_debug(operations->user_data,
        back_buffer_format, state->back_buffer_surface);
    state->previous_display_mode = state->display_mode_selection;
    state->previous_color_mode = state->color_mode_selection;
    state->mode_status_byte = 0;
    return 1;
}
