#include "pharaoh/runtime/application_window_messages.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "pharaoh/runtime/graphics_refresh.h"
#include "pharaoh/runtime/graphics_startup.h"

int32_t pharaoh_application_window_record_close_message(
    uint32_t message, uint8_t *native_close_state)
{
    if (message != PHARAOH_WINDOW_MESSAGE_CLOSE) {
        return 0;
    }

    *native_close_state = PHARAOH_NATIVE_WINDOW_CLOSE_REQUESTED;
    return 1;
}

int32_t pharaoh_application_window_handle_paint_message(
    uint32_t message, void *window,
    const PharaohApplicationWindowPaintContext *context)
{
    if (message != WM_PAINT) {
        return 0;
    }

    PAINTSTRUCT paint;
    (void)BeginPaint((HWND)window, &paint);
    (void)EndPaint((HWND)window, &paint);
    pharaoh_graphics_refresh_screen(context->graphics_state,
        context->graphics_operations, 1);
    *context->redraw_requested = 1;
    *context->city_redraw_requested = 1;
    return 1;
}

int32_t pharaoh_application_window_handle_destroy_message(
    uint32_t message,
    const PharaohApplicationWindowDestroyContext *context)
{
    if (message != WM_DESTROY) {
        return 0;
    }

    pharaoh_smack_video_shutdown(context->video_shutdown);
    *context->native_window_slot = NULL;
    pharaoh_graphics_session_release(context->graphics_state,
        context->graphics_operations, 1);
    PostQuitMessage(0);
    return 1;
}

int32_t pharaoh_application_window_handle_move_size_message(
    uint32_t message,
    const PharaohApplicationWindowMoveSizeContext *context)
{
    if (message != WM_MOVE && message != WM_SIZE) {
        return 0;
    }

    if (IsIconic((HWND)context->window) == 0) {
        pharaoh_application_window_clear_pause_if_foreground(
            context->window, context->paused_state);
        *context->activation_refresh_pending = 1;
    } else {
        pharaoh_application_window_pause_if_minimized(context->window,
            context->paused_state, context->report_user_data,
            context->report_paused);
    }

    if (context->fullscreen_display_mode == 0) {
        RECT *const bounds = (RECT *)context->screen_bounds;
        (void)GetClientRect((HWND)context->window, bounds);
        (void)ClientToScreen((HWND)context->window,
            (POINT *)&bounds->left);
        (void)ClientToScreen((HWND)context->window,
            (POINT *)&bounds->right);
    } else {
        context->screen_bounds->left = 0;
        context->screen_bounds->top = 0;
        context->screen_bounds->right = GetSystemMetrics(SM_CXSCREEN);
        context->screen_bounds->bottom = GetSystemMetrics(SM_CYSCREEN);
    }

    pharaoh_graphics_refresh_screen(context->graphics_state,
        context->graphics_operations, 1);
    *context->city_redraw_requested = 1;
    *context->redraw_requested = 1;
    return 1;
}

void pharaoh_application_window_clear_pause_if_foreground(
    void *window, int32_t *paused_state)
{
    if (GetForegroundWindow() == (HWND)window) {
        *paused_state = 0;
    }
}

void pharaoh_application_window_pause_if_minimized(
    void *window, int32_t *paused_state, void *report_user_data,
    void (*report_paused)(void *user_data))
{
    report_paused(report_user_data);
    *paused_state = 1;
    (void)InvalidateRect((HWND)window, NULL, TRUE);
}

