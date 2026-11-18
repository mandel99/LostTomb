#ifndef PHARAOH_RUNTIME_ANIMATION_FRAME_STATE_H
#define PHARAOH_RUNTIME_ANIMATION_FRAME_STATE_H

#include <stddef.h>
#include <stdint.h>

enum {
    PHARAOH_ANIMATION_FRAME_SLOT_COUNT = 51,
    PHARAOH_ANIMATION_FRAME_STAGGER_TICKS = 20
};

typedef struct PharaohAnimationFrameState {
    uint32_t last_update_tick[PHARAOH_ANIMATION_FRAME_SLOT_COUNT];
    uint8_t update_due[PHARAOH_ANIMATION_FRAME_SLOT_COUNT];
} PharaohAnimationFrameState;

_Static_assert(offsetof(PharaohAnimationFrameState, update_due) == 204,
               "native animation timestamp array extent");
_Static_assert(offsetof(PharaohAnimationFrameState, update_due) +
               sizeof(((PharaohAnimationFrameState *)0)->update_due) == 255,
               "native animation update-flag array extent");

/* Original entry 0x0040F270. Updates the 51 staggered animation slots using
   the native signed 32-bit deadline comparison. */
void pharaoh_animation_frame_state_update(
    PharaohAnimationFrameState *state, uint32_t current_tick);

#endif
