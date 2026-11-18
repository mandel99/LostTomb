#include "pharaoh/runtime/animation_frame_state.h"

static int32_t tick_at_or_after(uint32_t current_tick, uint32_t deadline)
{
    const uint32_t sign_bit = UINT32_C(0x80000000);
    const uint32_t sign_difference = (current_tick ^ deadline) & sign_bit;

    if (sign_difference != 0) {
        return (deadline & sign_bit) != 0;
    }
    return current_tick >= deadline;
}

void pharaoh_animation_frame_state_update(
    PharaohAnimationFrameState *state, uint32_t current_tick)
{
    for (uint32_t slot = 0;
         slot < PHARAOH_ANIMATION_FRAME_SLOT_COUNT; ++slot) {
        state->update_due[slot] = 0;
    }

    for (uint32_t slot = 0;
         slot < PHARAOH_ANIMATION_FRAME_SLOT_COUNT; ++slot) {
        const uint32_t stagger =
            slot * PHARAOH_ANIMATION_FRAME_STAGGER_TICKS;
        const uint32_t deadline = state->last_update_tick[slot] + stagger;

        if (tick_at_or_after(current_tick, deadline) != 0) {
            state->update_due[slot] = 1;
            state->last_update_tick[slot] = current_tick;
        }
    }
}
