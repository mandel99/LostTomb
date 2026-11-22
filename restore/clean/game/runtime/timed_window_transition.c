#include "pharaoh/runtime/timed_window_transition.h"

#include <string.h>

static int32_t signed_tick_from_native_bits(uint32_t tick_bits)
{
    int32_t tick;
    memcpy(&tick, &tick_bits, sizeof(tick));
    return tick;
}

void pharaoh_timed_window_transition_start(
    const PharaohTimedWindowTransitionStartContext *context,
    uint32_t transition_value, uint8_t next_window_id,
    int32_t duration_ms)
{
    PharaohTimedWindowTransitionContext *const transition =
        context->transition;
    const int32_t current_tick = signed_tick_from_native_bits(
        transition->get_tick_count(transition->clock_user_data));
    *context->transition_value = transition_value;
    transition->started_tick = current_tick;
    transition->deadline_tick = signed_tick_from_native_bits(
        (uint32_t)current_tick + (uint32_t)duration_ms);
    *context->next_window_storage = next_window_id;
    *transition->current_window_id = 4;
    *transition->redraw_requested = 1;
}

void pharaoh_timed_window_transition_dispatch(
    const PharaohTimedWindowTransitionContext *context)
{
    const int32_t current_tick = signed_tick_from_native_bits(
        context->get_tick_count(context->clock_user_data));

    if (context->deadline_tick < current_tick ||
        *context->advance_input != 0) {
        *context->redraw_requested = 1;
        *context->current_window_id = *context->next_window_id;
    }
}
