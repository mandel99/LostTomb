#include "pharaoh/runtime/frame_gate.h"

bool pharaoh_frame_gate_run(
    const PharaohFrameGateState *state,
    const PharaohFrameGateHooks *hooks)
{
    bool surface_lost = false;

    if (state->front_surface_available) {
        surface_lost = hooks->surface_is_lost(
            hooks->context, PHARAOH_DISPLAY_SURFACE_FRONT);
    }

    if (!surface_lost && state->back_surface_available) {
        surface_lost = hooks->surface_is_lost(
            hooks->context, PHARAOH_DISPLAY_SURFACE_BACK);
    }

    if (surface_lost && !hooks->restore_surfaces(hooks->context)) {
        hooks->pause_game(hooks->context);
        return false;
    }

    if (hooks->quit_requested(hooks->context)) {
        return false;
    }

    hooks->dispatch_frame(hooks->context);
    return true;
}
