#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "pharaoh/platform/windows_startup_host.h"
#include "pharaoh/platform/windows_startup_dispatch.h"
#include "pharaoh/platform/windows_frame_gate.h"
#include "pharaoh/runtime/application_message_loop.h"
#include "pharaoh/runtime/status_log_format.h"
#include "pharaoh/runtime/cursor_selection.h"

static PharaohWindowsStartupHost startup_host;
static PharaohWindowsStartupDispatch startup_dispatch;
static int32_t dispatcher_bound;
static int32_t idle_blocked;
static int32_t message_loop_active = 1;
static int32_t paused;

static void *set_game_cursor(void *context, void *cursor)
{
    (void)context;
    return SetCursor((HCURSOR)cursor);
}

static void report_paused(void *user_data)
{
    PharaohWindowsStartupHost *const host = user_data;
    pharaoh_status_log_format("OK: Application paused", NULL, 0,
        &host->log_format_state, &host->log_format_operations);
}

static void dispatch_boot_frame(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    pharaoh_windows_startup_dispatch_frame(dispatch);
}

/* Temporary host procedure until native 0x00414CD0 and the frame dispatcher
   have complete typed C owners. */
static LRESULT CALLBACK bootstrap_window_proc(HWND window, UINT message,
    WPARAM wparam, LPARAM lparam)
{
    if (message == WM_SETCURSOR && LOWORD(lparam) == HTCLIENT) {
        const PharaohCursorSelectionOperations cursor_operations = {
            NULL, set_game_cursor };
        pharaoh_cursor_select(&startup_host.window_startup.window,
            &cursor_operations, PHARAOH_CURSOR_SELECTOR_SLOT_00);
        return TRUE;
    }
    if (dispatcher_bound != 0) {
        pharaoh_windows_startup_dispatch_mouse(&startup_dispatch,
            message, lparam);
        if (message == WM_KEYDOWN && wparam == VK_ESCAPE) {
            pharaoh_windows_startup_dispatch_escape(&startup_dispatch);
            return 0;
        }
    }
    if (message == WM_SIZE) {
        idle_blocked = wparam == SIZE_MINIMIZED;
    }
    if (message == WM_CLOSE) {
        startup_host.shutdown_requested = 1;
        (void)DestroyWindow(window);
        /* A failed/delayed destruction must not leave the Miles stream and
           frame loop alive without a visible top-level window. */
        PostQuitMessage(0);
        return 0;
    }
    if (message == WM_WINDOWPOSCHANGED &&
        startup_host.window_startup.window.window == window) {
        (void)pharaoh_windows_startup_host_refresh_video_bounds(&startup_host);
    }
    if (message == WM_ERASEBKGND &&
        startup_host.graphics.primary_surface != NULL) {
        return 1;
    }
    if (message == WM_PAINT) {
        PAINTSTRUCT paint;
        (void)BeginPaint(window, &paint);
        if (startup_host.startup_video_started == 0) {
            pharaoh_windows_startup_host_present_loading(&startup_host);
        } else if (dispatcher_bound != 0 &&
            (startup_host.current_window_id == 0 ||
             startup_host.current_window_id == 0x3c)) {
            pharaoh_windows_startup_host_present_frame(&startup_host);
        }
        (void)EndPaint(window, &paint);
        return 0;
    }
    if (message == WM_DESTROY) {
        startup_host.shutdown_requested = 1;
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(window, message, wparam, lparam);
}

/* The captured Pharaoh entry is an ANSI WinMain with four x86 stack args. */
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR command_line,
    int show_command)
{
    PharaohWindowsFrameGate frame_gate = { 0 };
    PharaohFrameGateState gate_state = { 0 };
    PharaohFrameGateHooks gate_hooks = { 0 };
    PharaohApplicationMessageLoopContext loop = { 0 };
    HWND window;
    uint8_t post_close_requested = 0;
    (void)previous;
    (void)command_line;
    (void)show_command;
    if (pharaoh_windows_startup_host_load(&startup_host) !=
            PHARAOH_APP_CONFIGURATION_READY) {
        return 0;
    }

    if (pharaoh_windows_startup_host_open_window(&startup_host, instance,
            (PharaohApplicationWindowProcedure)bootstrap_window_proc) == 0) {
        return 0;
    }
    if (pharaoh_windows_startup_host_load_assets(&startup_host) == 0) {
        return 0;
    }
    if (pharaoh_windows_startup_host_load_pre_intro_data(&startup_host) == 0) {
        return 0;
    }
    startup_host.current_window_id = 0x44;
    (void)pharaoh_windows_sound_open(&startup_host.sound);
    pharaoh_windows_startup_host_finish_loading(&startup_host);
    if (startup_host.shutdown_requested == 0) {
        (void)pharaoh_windows_startup_host_play_intro(&startup_host);
    }

    if (startup_host.shutdown_requested != 0 ||
        pharaoh_windows_startup_dispatch_bind(&startup_dispatch,
            &startup_host) == 0) {
        pharaoh_windows_sound_close(&startup_host.sound);
        return 0;
    }
    startup_host.startup_video_started = 1;
    dispatcher_bound = 1;
    window = (HWND)startup_host.window_startup.window.window;
    frame_gate.restore.front_surface = startup_host.graphics.primary_surface;
    frame_gate.restore.back_surface = startup_host.graphics.back_buffer_surface;
    frame_gate.restore.status_state = &startup_host.log_format_state;
    frame_gate.restore.status_operations = &startup_host.log_format_operations;
    frame_gate.window = window;
    frame_gate.paused_state = &paused;
    frame_gate.report_user_data = &startup_host;
    frame_gate.report_paused = report_paused;
    frame_gate.quit_requested_state = &startup_host.shutdown_requested;
    frame_gate.dispatch_user_data = &startup_dispatch;
    frame_gate.dispatch_frame = dispatch_boot_frame;
    pharaoh_windows_frame_gate_bind(&frame_gate, &gate_state, &gate_hooks);
    loop.window = &window;
    loop.native_idle_frame_condition_first = &idle_blocked;
    loop.native_idle_frame_condition_second = &message_loop_active;
    loop.post_close_message_requested = &post_close_requested;
    loop.frame_gate_state = &gate_state;
    loop.frame_gate_hooks = &gate_hooks;
    const int32_t exit_code = pharaoh_application_message_loop_run(&loop);
    dispatcher_bound = 0;
    pharaoh_windows_startup_dispatch_close(&startup_dispatch);
    pharaoh_windows_sound_close(&startup_host.sound);
    return (int)exit_code;
}
