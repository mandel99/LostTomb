#ifndef PHARAOH_RUNTIME_PLATFORM_MEASURE_SPEED_H
#define PHARAOH_RUNTIME_PLATFORM_MEASURE_SPEED_H

#include "pharaoh/runtime/graphics_refresh.h"

#include <stdint.h>

typedef struct PharaohPlatformMeasurementState {
    int32_t display_mode;
    uint32_t idle_action_elapsed_ms;
    uint32_t graphics_refresh_elapsed_ms;
    uint32_t combined_elapsed_ms;
} PharaohPlatformMeasurementState;

typedef struct PharaohPlatformMeasurementOperations {
    void *user_data;
    uint32_t (*time_get_time)(void *user_data);
    PharaohGraphicsRefreshState *graphics_state;
    const PharaohGraphicsRefreshOperations *graphics_operations;
} PharaohPlatformMeasurementOperations;

/* Original entry 0x004CF8E0. The 0x004D0280 graphics-refresh routine remains
   a typed operation endpoint while its own stateful body is not yet ported. */
void pharaoh_platform_measure_speed(
    PharaohPlatformMeasurementState *state,
    const PharaohPlatformMeasurementOperations *operations);

/* Original entry 0x0051F780. Native body is an empty return; the argument is
   retained because 0x004CF8E0 pushes a mode-specific value at each callsite. */
void pharaoh_figure_action_none(uint32_t action_parameter);

#endif
