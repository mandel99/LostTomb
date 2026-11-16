#ifndef PHARAOH_RUNTIME_GRAPHICS_REFRESH_H
#define PHARAOH_RUNTIME_GRAPHICS_REFRESH_H

#include "pharaoh/core/static_objects.h"

#include <stdint.h>

typedef struct PharaohGraphicsRefreshState {
    int32_t skip_refresh;
    int32_t smacker_video_playing;
    int32_t smacker_uses_front_surface;
    uint8_t current_window_id;
    int32_t hailstorm_countdown;
    int32_t refresh_accumulated_ms;
    uint8_t force_refresh;
    uint32_t last_refresh_tick;
    PharaohScreenFade *screen_fade;
} PharaohGraphicsRefreshState;

typedef struct PharaohGraphicsRefreshOperations {
    void *user_data;
    uint32_t (*time_get_time)(void *user_data);
    void (*screen_fade_step)(void *user_data, PharaohScreenFade *fade,
                             int32_t buffer_index);
    void (*present_graphics)(void *user_data);
    void (*draw_sidebar_background)(void *user_data);
} PharaohGraphicsRefreshOperations;

/* Original entry 0x004D0040. Returns the wrapping DWORD tick delta. */
uint32_t pharaoh_graphics_millis_since_last(
    PharaohGraphicsRefreshState *state,
    const PharaohGraphicsRefreshOperations *operations);

/* Original entry 0x004D05F0. Its final step is an explicit typed operation
   because the native 0x004D0650 raster update remains a separate function. */
void pharaoh_screen_fade_start(
    PharaohGraphicsRefreshState *state, int32_t buffer_index,
    int32_t steps, int32_t level, int32_t jitter, int32_t mode,
    const PharaohGraphicsRefreshOperations *operations);

/* Original entry 0x004D0280. The present, sidebar draw, and fade-step calls
   are typed endpoints; timing, gates, counters, and refresh order are C. */
void pharaoh_graphics_refresh_screen(
    PharaohGraphicsRefreshState *state,
    const PharaohGraphicsRefreshOperations *operations,
    int32_t force_refresh);

#endif
