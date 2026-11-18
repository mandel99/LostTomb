#include "pharaoh/platform/windows_startup_dispatch.h"

#include <windowsx.h>
#include <shellapi.h>
#include <string.h>

static uint32_t frame_clock(void *user_data)
{
    (void)user_data;
    return GetTickCount();
}

static void present_frame(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    pharaoh_windows_startup_host_present_frame(dispatch->host);
}

static void clear_video_buffer(void *user_data)
{
    PharaohWindowsStartupHost *const host = user_data;
    (void)pharaoh_graphics_surfaces_clear(&host->graphics,
        &host->graphics_operations);
}

int32_t pharaoh_windows_startup_dispatch_bind(
    PharaohWindowsStartupDispatch *dispatch, PharaohWindowsStartupHost *host)
{
    memset(dispatch, 0, sizeof(*dispatch));
    dispatch->host = host;
    pharaoh_high_scores_bind_campaign(&dispatch->high_scores,
        &host->campaign);
    if (pharaoh_windows_smacker_open_library(&dispatch->smacker) == 0) {
        return 0;
    }
    dispatch->smacker.graphics = &host->graphics;
    dispatch->smacker.status_state = &host->log_format_state;
    dispatch->smacker.status_operations = &host->log_format_operations;
    pharaoh_windows_smacker_bind_operations(&dispatch->smacker,
        &dispatch->movie_open, &dispatch->movie_draw, &dispatch->movie_stop);
    dispatch->refresh.current_window_id = host->current_window_id;
    dispatch->refresh.last_refresh_tick = GetTickCount();
    dispatch->refresh_operations.user_data = dispatch;
    dispatch->refresh_operations.time_get_time = frame_clock;
    dispatch->refresh_operations.present_graphics = present_frame;
    dispatch->entry.entry_result_slot_0 = &dispatch->entry_results[0];
    dispatch->entry.entry_result_slot_1 = &dispatch->entry_results[1];
    dispatch->entry.entry_result_slot_2 = &dispatch->entry_results[2];
    dispatch->entry.entry_result_slot_3 = &dispatch->entry_results[3];
    dispatch->entry.input_mode = &dispatch->input_mode;
    dispatch->entry.redraw_requested = &host->graphics.redraw_requested;
    dispatch->input.mouse_x_ui = &dispatch->mouse_x_ui;
    dispatch->input.mouse_y_ui = &dispatch->mouse_y_ui;
    dispatch->transition.input = &dispatch->input;
    dispatch->transition.video = &dispatch->video;
    dispatch->video.redraw_requested = &host->graphics.redraw_requested;
    dispatch->video.graphics_refresh_state = &dispatch->refresh;
    dispatch->video.graphics_refresh_operations = &dispatch->refresh_operations;
    dispatch->video.display_surface_format =
        host->graphics.surface_context.pixel_format == 1 ? 0x565u : 0x555u;
    dispatch->video.audio_driver = NULL;
    memcpy(&dispatch->video.audio_driver, &host->sound.state.wave_device,
        sizeof(host->sound.state.wave_device));
    dispatch->video.movie_audio_setting = host->settings.sound_speech_enabled;
    dispatch->transition.entry_state = &dispatch->entry;
    dispatch->transition.screen_width = host->graphics.screen_width;
    dispatch->transition.screen_height = host->graphics.screen_height;
    dispatch->transition.display_context = host;
    dispatch->transition.clear_video_buffer = clear_video_buffer;
    dispatch->transition.video_operations = &dispatch->movie_open;
    dispatch->transition.video_stop_operations = &dispatch->movie_stop;
    dispatch->transition.video_dialog_overlay_active = &dispatch->dialog_active;
    return 1;
}

void pharaoh_windows_startup_dispatch_close(
    PharaohWindowsStartupDispatch *dispatch)
{
    if (dispatch->transition.video != NULL &&
        dispatch->transition.video->playback_active != 0) {
        pharaoh_smacker_window_video_stop(dispatch->transition.video,
            &dispatch->movie_stop);
    }
    pharaoh_windows_smacker_close_library(&dispatch->smacker);
}

void pharaoh_windows_startup_dispatch_mouse(
    PharaohWindowsStartupDispatch *dispatch, UINT message, LPARAM lparam)
{
    if (message == WM_MOUSEMOVE || message == WM_LBUTTONDOWN ||
        message == WM_RBUTTONDOWN || message == WM_LBUTTONUP || message == WM_RBUTTONUP) {
        dispatch->input.mouse_x = GET_X_LPARAM(lparam);
        dispatch->input.mouse_y = GET_Y_LPARAM(lparam);
    }
    if (message == WM_LBUTTONDOWN) {
        dispatch->left_press_active = 1;
        dispatch->left_press_window = dispatch->refresh.current_window_id;
        dispatch->left_pressed_this_frame = 1;
        if (dispatch->host != NULL &&
            dispatch->host->settings.sound_effects_enabled != 0) {
            pharaoh_windows_sound_button(&dispatch->host->sound,
                dispatch->host->settings.sound_effects_volume);
        }
    }
    if (message == WM_RBUTTONDOWN) {
        dispatch->right_press_active = 1;
        dispatch->right_press_window = dispatch->refresh.current_window_id;
    }
    if (message == WM_LBUTTONUP) {
        if (dispatch->left_press_active != 0 &&
            dispatch->left_press_window == dispatch->refresh.current_window_id) {
            dispatch->input.mouse_left_went_up = 1;
            dispatch->input.video_input_flag_a = 1;
        }
        dispatch->left_press_active = 0;
    }
    if (message == WM_RBUTTONUP) {
        if (dispatch->right_press_active != 0 &&
            dispatch->right_press_window == dispatch->refresh.current_window_id) {
            dispatch->input.mouse_right_went_up = 1;
            dispatch->input.video_input_flag_b = 1;
        }
        dispatch->right_press_active = 0;
    }
}

void pharaoh_windows_startup_dispatch_frame(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    PharaohSmackerWindowVideoState *const video = dispatch->transition.video;
    const uint8_t previous_window = dispatch->refresh.current_window_id;
    const int32_t previous_video_target = video->return_window_id;
    /* Window-zero drawing and hover; the full menu action chain remains a
       separate owner and is not replaced with invented campaign behavior. */
    if (dispatch->refresh.current_window_id == 0) {
        pharaoh_windows_sound_menu_music(&dispatch->host->sound,
            dispatch->host->settings.sound_music_enabled,
            dispatch->host->settings.sound_music_volume);
        PharaohGenericButtonFocusState focus = { 0 };
        const int32_t focused = pharaoh_generic_buttons_get_focus(
            (dispatch->host->graphics.render_width - 640) / 2,
            (dispatch->host->graphics.render_height - 480) / 2,
            pharaoh_front_menu_rects, PHARAOH_MENU_BUTTON_COUNT,
            dispatch->input.mouse_x, dispatch->input.mouse_y, &focus);
        if (focused != dispatch->menu_focused_button ||
            dispatch->host->graphics.redraw_requested != 0) {
            pharaoh_windows_startup_host_draw_boot_background(dispatch->host, 0);
            pharaoh_windows_startup_host_draw_boot_foreground(dispatch->host, focused);
            dispatch->menu_focused_button = focused;
        }
        if (focused == PHARAOH_MENU_BUTTON_COUNT &&
            dispatch->left_pressed_this_frame != 0 &&
            dispatch->left_press_window == 0) {
            (void)PostMessageA((HWND)dispatch->host->window_startup.window.window,
                WM_KEYDOWN, VK_ESCAPE, 0);
        } else if (focused == 2 && dispatch->left_pressed_this_frame != 0 &&
                dispatch->left_press_window == 0) {
            HWND window = (HWND)dispatch->host->window_startup.window.window;
            (void)ShowWindow(window, SW_MINIMIZE);
            (void)ShellExecuteA(window, "open", "www.gog.com", NULL, NULL,
                SW_SHOWNORMAL);
        }
        dispatch->left_pressed_this_frame = 0;
        dispatch->input.mouse_left_went_up = 0;
        dispatch->input.mouse_right_went_up = 0;
        dispatch->input.video_input_flag_a = 0;
        dispatch->input.video_input_flag_b = 0;
        pharaoh_graphics_refresh_screen(&dispatch->refresh,
            &dispatch->refresh_operations, 0);
        return;
    }
    if (dispatch->refresh.current_window_id == 0x3c &&
        dispatch->host->graphics.redraw_requested != 0) {
        pharaoh_windows_sound_menu_music(&dispatch->host->sound,
            dispatch->host->settings.sound_music_enabled,
            dispatch->host->settings.sound_music_volume);
        pharaoh_windows_startup_host_draw_boot_background(dispatch->host, 0x3c);
        pharaoh_windows_startup_host_draw_boot_foreground(dispatch->host, 0);
    }
    /* Draw before input, as at 004D6C62/004D6C67. A natural end may select
       the next video, which the input dispatcher starts in this same frame. */
    if (dispatch->refresh.current_window_id == 0x13) {
        (void)pharaoh_smacker_video_draw(video, &dispatch->movie_draw);
    }
    if (previous_window != dispatch->refresh.current_window_id) {
        /* A release collected for the movie must not advance the screen
           selected by its natural end in this same frame. */
        dispatch->left_press_active = 0;
        dispatch->right_press_active = 0;
        dispatch->input.mouse_left_went_up = 0;
        dispatch->input.mouse_right_went_up = 0;
        dispatch->input.video_input_flag_a = 0;
        dispatch->input.video_input_flag_b = 0;
    }
    pharaoh_startup_window_transition_dispatch(&dispatch->transition);
    dispatch->host->current_window_id = dispatch->refresh.current_window_id;
    if (previous_window != dispatch->refresh.current_window_id ||
        previous_video_target != video->return_window_id) {
        dispatch->left_press_active = 0;
        dispatch->right_press_active = 0;
    }
    if (dispatch->refresh.current_window_id == 0) {
        pharaoh_windows_startup_host_draw_boot_background(dispatch->host, 0);
        pharaoh_windows_startup_host_draw_boot_foreground(dispatch->host, 0);
    }
    dispatch->input.mouse_left_went_up = 0;
    dispatch->input.mouse_right_went_up = 0;
    dispatch->input.video_input_flag_a = 0;
    dispatch->input.video_input_flag_b = 0;
    dispatch->left_pressed_this_frame = 0;
    pharaoh_graphics_refresh_screen(&dispatch->refresh,
        &dispatch->refresh_operations, 0);
}
