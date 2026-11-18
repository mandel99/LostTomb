#ifndef PHARAOH_RUNTIME_APPLICATION_MESSAGE_LOOP_H
#define PHARAOH_RUNTIME_APPLICATION_MESSAGE_LOOP_H

#include "pharaoh/runtime/frame_gate.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <stdint.h>

typedef struct PharaohApplicationMessageLoopContext {
    const HWND *window;
    const int32_t *native_idle_frame_condition_first;
    const int32_t *native_idle_frame_condition_second;
    uint8_t *post_close_message_requested;
    const PharaohFrameGateState *frame_gate_state;
    const PharaohFrameGateHooks *frame_gate_hooks;
} PharaohApplicationMessageLoopContext;

/* Recreates the Win32 message/frame-pump portion of native 0x00413ED0.
   The native teardown after WM_QUIT remains a separate startup owner. */
int32_t pharaoh_application_message_loop_run(
    const PharaohApplicationMessageLoopContext *context);

#endif
