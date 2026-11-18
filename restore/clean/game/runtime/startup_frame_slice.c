#include "pharaoh/runtime/startup_frame_slice.h"

#include "pharaoh/runtime/graphics_refresh.h"

void pharaoh_startup_frame_slice(
    const PharaohStartupFrameSliceContext *context)
{
    PharaohScreenBackgroundDispatchContext background =
        *context->screen_background;
    PharaohStartupForegroundSliceContext foreground = *context->foreground;
    PharaohSmackerWindowVideoState *const video =
        context->window_transition->video;
    const uint8_t window_id = video->graphics_refresh_state->current_window_id;

    (void)pharaoh_mouse_update(context->mouse_state,
        context->mouse_operations, context->sound_effect_state,
        context->sound_effect_operations);

    background.current_window_id = window_id;
    pharaoh_screen_background_dispatch(&background, 0);

    pharaoh_view_invalidate_if_editing(context->view_state);

    foreground.current_window_id = window_id;
    pharaoh_startup_foreground_slice(&foreground);

    pharaoh_startup_window_transition_dispatch(context->window_transition);

    pharaoh_input_transient_reset(context->transient_input);
}
