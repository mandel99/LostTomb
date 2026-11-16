#include "pharaoh/runtime/graphics_refresh.h"

#include <string.h>

enum {
    PHARAOH_HAILSTORM_WINDOW_ID = 5,
    PHARAOH_HAILSTORM_REFRESH_PERIOD = 0x88,
    PHARAOH_GRAPHICS_REFRESH_INTERVAL_MS = 0x32,
    PHARAOH_HAILSTORM_FADE_BUFFER_INDEX = 1,
    PHARAOH_HAILSTORM_FADE_STEPS = 15,
    PHARAOH_HAILSTORM_FADE_LEVEL = 0,
    PHARAOH_HAILSTORM_FADE_JITTER = 1,
    PHARAOH_HAILSTORM_FADE_MODE = 0,
    PHARAOH_HAILSTORM_FADE_STEP_COUNT = 8,
    PHARAOH_WINDOW_FADE_BUFFER_INDEX = 1,
    PHARAOH_WINDOW_FADE_STEPS = 1,
    PHARAOH_WINDOW_FADE_LEVEL = 0,
    PHARAOH_WINDOW_FADE_JITTER = 0,
    PHARAOH_WINDOW_FADE_MODE = 1,
    PHARAOH_WINDOW_ID_1 = 1,
    PHARAOH_WINDOW_ID_3 = 3,
    PHARAOH_WINDOW_ID_30 = 30,
    PHARAOH_WINDOW_ID_31 = 31,
    PHARAOH_WINDOW_ID_32 = 32,
    PHARAOH_WINDOW_ID_73 = 73
};

static int32_t window_draws_sidebar_background(uint8_t window_id)
{
    switch (window_id) {
    case PHARAOH_WINDOW_ID_1:
    case PHARAOH_WINDOW_ID_3:
    case PHARAOH_WINDOW_ID_30:
    case PHARAOH_WINDOW_ID_31:
    case PHARAOH_WINDOW_ID_32:
    case PHARAOH_WINDOW_ID_73:
        return 1;
    default:
        return 0;
    }
}

uint32_t pharaoh_graphics_millis_since_last(
    PharaohGraphicsRefreshState *state,
    const PharaohGraphicsRefreshOperations *operations)
{
    const uint32_t current_tick = operations->time_get_time(
        operations->user_data);
    const uint32_t elapsed = current_tick - state->last_refresh_tick;
    state->last_refresh_tick = current_tick;
    return elapsed;
}

void pharaoh_screen_fade_start(
    PharaohGraphicsRefreshState *state, int32_t buffer_index,
    int32_t steps, int32_t level, int32_t jitter, int32_t mode,
    const PharaohGraphicsRefreshOperations *operations)
{
    if (steps < 0) {
        state->screen_fade->steps = 0;
    } else if (steps > 15) {
        state->screen_fade->steps = 15;
    } else {
        state->screen_fade->steps = steps;
    }
    state->screen_fade->level = level;
    state->screen_fade->jitter = (uint8_t)jitter;
    state->screen_fade->mode = mode;
    operations->screen_fade_step(operations->user_data,
        state->screen_fade, buffer_index);
}

static void screen_fade_start_for_hailstorm(
    PharaohGraphicsRefreshState *state,
    const PharaohGraphicsRefreshOperations *operations)
{
    pharaoh_screen_fade_start(state, PHARAOH_HAILSTORM_FADE_BUFFER_INDEX,
        PHARAOH_HAILSTORM_FADE_STEPS, PHARAOH_HAILSTORM_FADE_LEVEL,
        PHARAOH_HAILSTORM_FADE_JITTER, PHARAOH_HAILSTORM_FADE_MODE,
        operations);
}

static void screen_fade_start_for_window(
    PharaohGraphicsRefreshState *state,
    const PharaohGraphicsRefreshOperations *operations)
{
    pharaoh_screen_fade_start(state, PHARAOH_WINDOW_FADE_BUFFER_INDEX,
        PHARAOH_WINDOW_FADE_STEPS, PHARAOH_WINDOW_FADE_LEVEL,
        PHARAOH_WINDOW_FADE_JITTER, PHARAOH_WINDOW_FADE_MODE, operations);
}

static void refresh_hailstorm_transition(
    PharaohGraphicsRefreshState *state,
    const PharaohGraphicsRefreshOperations *operations)
{
    screen_fade_start_for_hailstorm(state, operations);
    operations->present_graphics(operations->user_data);
    for (int32_t step = 0; step < PHARAOH_HAILSTORM_FADE_STEP_COUNT; ++step) {
        operations->screen_fade_step(operations->user_data,
            state->screen_fade, PHARAOH_HAILSTORM_FADE_BUFFER_INDEX);
        operations->present_graphics(operations->user_data);
    }
}

void pharaoh_graphics_refresh_screen(
    PharaohGraphicsRefreshState *state,
    const PharaohGraphicsRefreshOperations *operations,
    int32_t force_refresh)
{
    if (state->skip_refresh == 1 ||
        (state->smacker_video_playing != 0 &&
            state->smacker_uses_front_surface != 0)) {
        return;
    }

    if (state->current_window_id != PHARAOH_HAILSTORM_WINDOW_ID &&
        state->hailstorm_countdown > 0 &&
        state->hailstorm_countdown % PHARAOH_HAILSTORM_REFRESH_PERIOD == 0) {
        refresh_hailstorm_transition(state, operations);
        return;
    }

    {
        const uint32_t accumulated_bits =
            (uint32_t)state->refresh_accumulated_ms +
            pharaoh_graphics_millis_since_last(state, operations);
        memcpy(&state->refresh_accumulated_ms, &accumulated_bits,
            sizeof(accumulated_bits));
    }
    if (force_refresh == 1) {
        state->refresh_accumulated_ms = PHARAOH_GRAPHICS_REFRESH_INTERVAL_MS;
    }

    if (state->force_refresh == 0 &&
        state->refresh_accumulated_ms < PHARAOH_GRAPHICS_REFRESH_INTERVAL_MS) {
        return;
    }

    state->refresh_accumulated_ms = 0;
    state->force_refresh = 0;
    if (state->current_window_id != PHARAOH_HAILSTORM_WINDOW_ID &&
        state->hailstorm_countdown > 0 &&
        window_draws_sidebar_background(state->current_window_id) != 0) {
        operations->draw_sidebar_background(operations->user_data);
        screen_fade_start_for_window(state, operations);
    }
    operations->present_graphics(operations->user_data);
}
