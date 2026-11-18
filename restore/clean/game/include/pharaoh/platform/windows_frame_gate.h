#ifndef PHARAOH_PLATFORM_WINDOWS_FRAME_GATE_H
#define PHARAOH_PLATFORM_WINDOWS_FRAME_GATE_H

#include "pharaoh/runtime/frame_gate.h"
#include "pharaoh/runtime/graphics_frame_restore.h"

typedef struct PharaohWindowsFrameGate {
    PharaohGraphicsFrameRestoreContext restore;
    PharaohGraphicsFrameRestoreOperations restore_operations;
    void *window;
    int32_t *paused_state;
    void *report_user_data;
    void (*report_paused)(void *user_data);
    const uint8_t *quit_requested_state;
    void *dispatch_user_data;
    void (*dispatch_frame)(void *user_data);
} PharaohWindowsFrameGate;

/* The caller supplies the game frame owner and initialized DirectDraw surfaces. */
void pharaoh_windows_frame_gate_bind(PharaohWindowsFrameGate *context,
    PharaohFrameGateState *state, PharaohFrameGateHooks *hooks);

#endif
