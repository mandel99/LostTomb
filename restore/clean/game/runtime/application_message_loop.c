#include "pharaoh/runtime/application_message_loop.h"

int32_t pharaoh_application_message_loop_run(
    const PharaohApplicationMessageLoopContext *context)
{
    MSG message = { 0 };

    for (;;) {
        if (PeekMessageA(&message, NULL, 0, 0, 0) == 0) {
            if (*context->native_idle_frame_condition_first == 0 &&
                *context->native_idle_frame_condition_second != 0) {
                (void)pharaoh_frame_gate_run(context->frame_gate_state,
                    context->frame_gate_hooks);
            } else {
                (void)WaitMessage();
            }
        } else {
            const BOOL message_result =
                GetMessageA(&message, NULL, 0, 0);
            if (message_result == 0) {
                break;
            }
            (void)TranslateMessage(&message);
            (void)DispatchMessageA(&message);
        }

        if (*context->post_close_message_requested == 1) {
            (void)PostMessageA(*context->window, WM_CLOSE, 0, 0);
            *context->post_close_message_requested = 0;
        }
    }

    return (int32_t)message.wParam;
}
