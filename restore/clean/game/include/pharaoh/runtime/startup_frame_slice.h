#ifndef PHARAOH_RUNTIME_STARTUP_FRAME_SLICE_H
#define PHARAOH_RUNTIME_STARTUP_FRAME_SLICE_H

#include "pharaoh/runtime/input_transient_reset.h"
#include "pharaoh/runtime/mouse_update.h"
#include "pharaoh/runtime/screen_background_dispatch.h"
#include "pharaoh/runtime/startup_foreground_slice.h"
#include "pharaoh/runtime/startup_window_transition.h"
#include "pharaoh/ui/view_invalidation.h"

typedef struct PharaohStartupFrameSliceContext {
    PharaohMouseUpdateState *mouse_state;
    const PharaohMouseUpdateOperations *mouse_operations;
    PharaohSoundEffectRequestState *sound_effect_state;
    const PharaohSoundEffectRequestOperations *sound_effect_operations;
    PharaohScreenBackgroundDispatchContext *screen_background;
    PharaohViewState *view_state;
    PharaohStartupForegroundSliceContext *foreground;
    const PharaohStartupWindowTransitionContext *window_transition;
    PharaohInputTransientResetState *transient_input;
} PharaohStartupFrameSliceContext;

/* Startup-window slice of native frame dispatcher 0x004D6B60. Preserves the
   observed mouse, background, view, foreground, window, transient-reset order.
   General simulation, cursor/audio maintenance, and final screen refresh stay
   with the not-yet-complete full dispatcher. */
void pharaoh_startup_frame_slice(
    const PharaohStartupFrameSliceContext *context);

#endif
