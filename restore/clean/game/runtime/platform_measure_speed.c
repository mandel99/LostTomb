#include "pharaoh/runtime/platform_measure_speed.h"

enum {
    PHARAOH_DISPLAY_MODE_FIRST = 1,
    PHARAOH_DISPLAY_MODE_SECOND = 2,
    PHARAOH_DISPLAY_MODE_THIRD = 3,
    PHARAOH_ACTION_PARAMETER_FIRST_MODE = 0x14,
    PHARAOH_ACTION_PARAMETER_SECOND_MODE = 0x1e,
    PHARAOH_ACTION_PARAMETER_THIRD_MODE = 0x28,
    PHARAOH_PLATFORM_MEASUREMENT_ITERATIONS = 3
};

static int32_t get_action_parameter(int32_t display_mode,
    uint32_t *action_parameter)
{
    switch (display_mode) {
    case PHARAOH_DISPLAY_MODE_FIRST:
        *action_parameter = PHARAOH_ACTION_PARAMETER_FIRST_MODE;
        return 1;
    case PHARAOH_DISPLAY_MODE_SECOND:
        *action_parameter = PHARAOH_ACTION_PARAMETER_SECOND_MODE;
        return 1;
    case PHARAOH_DISPLAY_MODE_THIRD:
        *action_parameter = PHARAOH_ACTION_PARAMETER_THIRD_MODE;
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
    uint32_t action_parameter = 0;
    uint32_t first_end;
    uint32_t refresh_start;
    uint32_t refresh_end;

    if (get_action_parameter(state->display_mode, &action_parameter) != 0) {
        for (uint32_t iteration = 0;
            iteration < PHARAOH_PLATFORM_MEASUREMENT_ITERATIONS;
            ++iteration) {
            pharaoh_figure_action_none(action_parameter);
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
