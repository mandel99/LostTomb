#include "pharaoh/runtime/timed_window_transition.h"

#include <string.h>

static int32_t signed_tick_from_native_bits(uint32_t tick_bits)
{
    int32_t tick;
    memcpy(&tick, &tick_bits, sizeof(tick));
    return tick;
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
