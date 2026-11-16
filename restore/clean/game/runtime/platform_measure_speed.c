#include "pharaoh/runtime/platform_measure_speed.h"

enum {
    PHARAOH_DISPLAY_MODE_FIRST = 1,
    PHARAOH_DISPLAY_MODE_SECOND = 2,
    PHARAOH_DISPLAY_MODE_THIRD = 3,
    PHARAOH_PLATFORM_MEASUREMENT_ITERATIONS = 3
};

static int32_t has_native_idle_action_mode(int32_t display_mode)
{
    switch (display_mode) {
    case PHARAOH_DISPLAY_MODE_FIRST:
    case PHARAOH_DISPLAY_MODE_SECOND:
    case PHARAOH_DISPLAY_MODE_THIRD:
        return 1;
    default:
        return 0;
    }
}

void pharaoh_platform_measure_speed(
    PharaohPlatformMeasurementState *state,
    const PharaohPlatformMeasurementOperations *operations)
{
    const uint32_t first_start = operations->time_get_time(
        operations->user_data);
    uint32_t first_end;
    uint32_t refresh_start;
    uint32_t refresh_end;

    if (has_native_idle_action_mode(state->display_mode) != 0) {
        for (uint32_t iteration = 0;
            iteration < PHARAOH_PLATFORM_MEASUREMENT_ITERATIONS;
            ++iteration) {
            pharaoh_actor_walking_no_operation();
        }
    }

    first_end = operations->time_get_time(operations->user_data);
    state->idle_action_elapsed_ms = first_end - first_start;
    refresh_start = operations->time_get_time(operations->user_data);

    for (uint32_t iteration = 0;
        iteration < PHARAOH_PLATFORM_MEASUREMENT_ITERATIONS;
        ++iteration) {
        pharaoh_graphics_refresh_screen(operations->graphics_state,
            operations->graphics_operations, 1);
    }

    refresh_end = operations->time_get_time(operations->user_data);
    state->graphics_refresh_elapsed_ms = refresh_end - refresh_start;
    state->combined_elapsed_ms = state->graphics_refresh_elapsed_ms +
        state->idle_action_elapsed_ms;
}
