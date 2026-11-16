#ifndef PHARAOH_RUNTIME_FRAME_GATE_H
#define PHARAOH_RUNTIME_FRAME_GATE_H

#include <stdbool.h>

typedef enum PharaohDisplaySurface {
    PHARAOH_DISPLAY_SURFACE_FRONT = 0,
    PHARAOH_DISPLAY_SURFACE_BACK = 1
} PharaohDisplaySurface;

typedef struct PharaohFrameGateState {
    bool front_surface_available;
    bool back_surface_available;
} PharaohFrameGateState;

typedef struct PharaohFrameGateHooks {
    void *context;
    bool (*surface_is_lost)(void *context, PharaohDisplaySurface surface);
    bool (*restore_surfaces)(void *context);
    void (*pause_game)(void *context);
    bool (*quit_requested)(void *context);
    void (*dispatch_frame)(void *context);
} PharaohFrameGateHooks;

/* Original address 0x004143C0. Returns true only when a frame was dispatched. */
bool pharaoh_frame_gate_run(
    const PharaohFrameGateState *state,
    const PharaohFrameGateHooks *hooks);

#endif
