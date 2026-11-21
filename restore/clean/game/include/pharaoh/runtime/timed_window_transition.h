#ifndef PHARAOH_RUNTIME_TIMED_WINDOW_TRANSITION_H
#define PHARAOH_RUNTIME_TIMED_WINDOW_TRANSITION_H

#include <stdint.h>

typedef struct PharaohTimedWindowTransitionContext {
    int32_t deadline_tick;
    const uint8_t *advance_input;
    uint8_t *current_window_id;
    const uint8_t *next_window_id;
    uint8_t *redraw_requested;
    void *clock_user_data;
    uint32_t (*get_tick_count)(void *user_data);
} PharaohTimedWindowTransitionContext;

typedef struct PharaohTimedWindowTransitionStartContext {
    PharaohTimedWindowTransitionContext *transition;
    uint32_t *transition_value;
    uint8_t *next_window_storage;
} PharaohTimedWindowTransitionStartContext;

/* Complete typed C translation of native 0x0041AA20. */
void pharaoh_timed_window_transition_start(
    const PharaohTimedWindowTransitionStartContext *context,
    uint32_t transition_value, uint8_t next_window_id,
    int32_t duration_ms);

/* Native helper 0x0041AA60, selected by dispatcher state 0x04. */
void pharaoh_timed_window_transition_dispatch(
    const PharaohTimedWindowTransitionContext *context);

#endif
