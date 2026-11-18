#include "pharaoh/platform/windows_startup_dispatch.h"
#include "pharaoh/ui/front_menu_action.h"
#include "pharaoh/ui/main_menu.h"

#include <windowsx.h>
#include <shellapi.h>
#include <stdio.h>
#include <string.h>

static const PharaohMenuRect player_selection_rects[16] = {
    {160,128,448,144}, {160,144,448,160}, {160,160,448,176},
    {160,176,448,192}, {160,192,448,208}, {160,208,448,224},
    {160,224,448,240}, {160,240,448,256}, {160,256,448,272},
    {160,272,448,288}, {160,288,448,304}, {160,304,448,320},
    {142,332,267,357}, {276,332,406,357}, {418,332,498,357},
    {196,362,446,387}
};
static const PharaohMenuRect saved_game_row_rects[12] = {
    {160,128,448,144}, {160,144,448,160}, {160,160,448,176},
    {160,176,448,192}, {160,192,448,208}, {160,208,448,224},
    {160,224,448,240}, {160,240,448,256}, {160,256,448,272},
    {160,272,448,288}, {160,288,448,304}, {160,304,448,320}
};

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

static void front_menu_open_player_selection(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    const int32_t previous_profile =
        dispatch->player_selection.selected_profile;
    const int32_t profile_count = pharaoh_player_selection_load_profiles(
        &dispatch->player_selection, previous_profile);
    dispatch->refresh.current_window_id = profile_count == 0 ? 0x36 : 0x2f;
    dispatch->host->graphics.redraw_requested = 1;
}

static void front_menu_show_support(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    HWND window = (HWND)dispatch->host->window_startup.window.window;
    (void)ShowWindow(window, SW_MINIMIZE);
    (void)ShellExecuteA(window, "open", "www.gog.com", NULL, NULL,
        SW_SHOWNORMAL);
}

static void front_menu_open_custom_maps(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    (void)pharaoh_custom_map_selection_load(&dispatch->custom_map_selection);
    dispatch->refresh.current_window_id = 0x3d;
    dispatch->host->graphics.redraw_requested = 1;
}

static void front_menu_open_high_scores(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    (void)pharaoh_high_scores_load(&dispatch->high_scores,
        &dispatch->high_scores_operations, "save\\highscore.jas");
    dispatch->host->graphics.redraw_requested = 1;
}

static void front_menu_request_exit(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    (void)PostMessageA((HWND)dispatch->host->window_startup.window.window,
        WM_KEYDOWN, VK_ESCAPE, 0);
}

static void exit_box_stop_video(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    if (dispatch->transition.video->playback_active != 0) {
        pharaoh_smacker_window_video_stop(dispatch->transition.video,
            &dispatch->movie_stop);
    }
}

static const char *exit_box_select_text(void *user_data, int32_t group,
    int32_t entry)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    pharaoh_text_select(&dispatch->host->text_selection, group, entry);
    return dispatch->host->text_selection.current;
}

static int32_t exit_box_has_message(void *user_data)
{
    MSG message;
    (void)user_data;
    return PeekMessageA(&message, NULL, 0, 0, PM_NOREMOVE) != 0;
}

static int32_t exit_box_get_dispatch_message(void *user_data)
{
    MSG message;
    (void)user_data;
    if (GetMessageA(&message, NULL, 0, 0) <= 0) return 0;
    (void)TranslateMessage(&message);
    (void)DispatchMessageA(&message);
    return 1;
}

static void exit_box_wait_message(void *user_data)
{
    (void)user_data;
    (void)WaitMessage();
}

static int32_t exit_box_run_frame(void *user_data, const char *title,
    const char *body, uint32_t type)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    PharaohMessageBoxState *const box = &dispatch->exit_message_box;
    PharaohMenuRect button_rects[PHARAOH_MESSAGE_BOX_MAX_BUTTONS];
    PharaohGenericButtonFocusState focus_state = { 0 };
    int32_t index;

    box->focused_button = 0;
    (void)pharaoh_windows_startup_host_draw_message_box(dispatch->host, box,
        title, body, type);
    for (index = 0; index < box->button_count; ++index) {
        button_rects[index].left = box->buttons[index].left;
        button_rects[index].top = box->buttons[index].top;
        button_rects[index].right = box->buttons[index].right;
        button_rects[index].bottom = box->buttons[index].bottom;
    }
    box->focused_button = pharaoh_generic_buttons_get_focus(
        0, 0, button_rects, box->button_count, dispatch->input.mouse_x,
        dispatch->input.mouse_y, &focus_state);
    if (box->focused_button != 0 &&
        dispatch->left_pressed_this_frame != 0 &&
        dispatch->left_press_window == 0x54) {
        pharaoh_message_box_select(box, box->focused_button - 1);
    }
    dispatch->left_pressed_this_frame = 0;
    dispatch->input.mouse_left_went_up = 0;
    dispatch->input.mouse_right_went_up = 0;
    (void)pharaoh_windows_startup_host_draw_message_box(dispatch->host, box,
        title, body, type);
    pharaoh_windows_startup_host_present_frame(dispatch->host);
    return box->activated != 0 ?
        box->buttons[box->selected_index].result : -1;
}

static void exit_box_post_close(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    (void)PostMessageA((HWND)dispatch->host->window_startup.window.window,
        WM_CLOSE, 0, 0);
}

static int32_t front_menu_show_exit_box(void *user_data, const char *title,
    const char *body, uint32_t type)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    PharaohMessageBoxState *const box = &dispatch->exit_message_box;
    PharaohMessageBoxModalState state;
    const PharaohMessageBoxModalOperations operations = {
        dispatch, exit_box_has_message, exit_box_get_dispatch_message,
        exit_box_wait_message, exit_box_run_frame, exit_box_post_close
    };
    int32_t result;

    memset(box, 0, sizeof(*box));
    box->screen_width = dispatch->host->graphics.render_width;
    box->screen_height = dispatch->host->graphics.render_height;
    /* These variants are also the menu's measured/normal/large draw fonts. */
    box->title_font = 640;
    box->body_font = 635;
    box->title_draw_font = 641;
    state.box = box;
    state.current_window_id = &dispatch->refresh.current_window_id;
    state.redraw_request = &dispatch->host->graphics.redraw_requested;
    state.post_close_requested = &dispatch->exit_post_close_requested;
    state.display_unavailable = &dispatch->message_box_display_unavailable;
    state.display_ready = &dispatch->message_box_display_ready;
    dispatch->exit_message_box_active = 1;
    dispatch->message_box_display_unavailable = 0;
    dispatch->message_box_display_ready = 1;
    dispatch->host->current_window_id = 0x54;
    result = pharaoh_message_box_show(&state, &operations, title, body, type);
    dispatch->exit_message_box_active = 0;
    dispatch->host->current_window_id = dispatch->refresh.current_window_id;
    return result;
}

static void front_menu_confirm_exit(PharaohWindowsStartupDispatch *dispatch)
{
    PharaohFrontMenuExitState state = {
        &dispatch->mouse_button_state,
        &dispatch->exit_post_close_requested,
        &dispatch->host->graphics.redraw_requested
    };
    const PharaohFrontMenuExitOperations operations = {
        dispatch, exit_box_stop_video, exit_box_select_text,
        exit_box_select_text, front_menu_show_exit_box
    };
    pharaoh_front_menu_confirm_exit(&state, &operations);
    if (dispatch->exit_post_close_requested != 0) {
        dispatch->exit_post_close_requested = 0;
        (void)PostMessageA((HWND)dispatch->host->window_startup.window.window,
            WM_CLOSE, 0, 0);
    }
    dispatch->left_press_active = 0;
    dispatch->right_press_active = 0;
    dispatch->host->current_window_id = dispatch->refresh.current_window_id;
}

static PharaohReadOnlyFile *main_menu_open_file(void *user_data,
    const char *filename, const char *mode)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    (void)mode;
    return dispatch->host->files.operations->open(
        dispatch->host->files.user_data, filename);
}

static int32_t main_menu_close_file(void *user_data,
    PharaohReadOnlyFile *file)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    dispatch->host->files.operations->close(
        dispatch->host->files.user_data, file);
    return 1;
}

static int32_t main_menu_count_matching_files(void *user_data,
    const char *pattern, int32_t search_mode)
{
    PharaohFileList files;
    int32_t count;
    (void)user_data;
    pharaoh_file_list_initialize(&files);
    count = pharaoh_file_list_load(&files, pattern, search_mode);
    pharaoh_file_list_destroy(&files);
    return count;
}

static void open_selected_player_menu(
    PharaohWindowsStartupDispatch *dispatch)
{
    const PharaohMainMenuOpenOperations operations = {
        dispatch,
        main_menu_open_file,
        main_menu_close_file,
        main_menu_count_matching_files
    };
    const PharaohMainMenuOpenContext context = {
        &dispatch->host->campaign.index,
        &dispatch->main_menu_window,
        dispatch->host->campaign.saved_game_path,
        dispatch->player_selection.player_name,
        &operations
    };

    pharaoh_main_menu_window_show(&context);
    dispatch->refresh.current_window_id =
        dispatch->main_menu_window.active_window;
    dispatch->host->current_window_id =
        dispatch->main_menu_window.active_window;
}

static void main_menu_stop_sound(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    pharaoh_digital_sound_stream_close(&dispatch->host->sound.state, 0,
        &dispatch->host->sound.stream_operations);
}

static int32_t main_menu_has_campaign_history(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    return pharaoh_campaign_any_mission_played(
        &dispatch->host->campaign.index);
}

static int32_t main_menu_confirm_campaign_restart(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    const char *const title = exit_box_select_text(dispatch, 5, 142);
    const char *const body = exit_box_select_text(dispatch, 5, 141);
    return front_menu_show_exit_box(dispatch, title, body, 4);
}

static void main_menu_open_map_selection(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    if (dispatch->menu_map_mode_704dac == 2) {
        (void)pharaoh_campaign_mission_selection_load(
            &dispatch->custom_map_selection, &dispatch->host->campaign);
    } else if (dispatch->menu_map_mode_704dac == 3) {
        (void)pharaoh_custom_map_selection_load(
            &dispatch->custom_map_selection);
    }
    dispatch->refresh.current_window_id = 0x3d;
    dispatch->host->current_window_id = 0x3d;
    dispatch->host->graphics.redraw_requested = 1;
}

static void main_menu_open_saved_game_dialog(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    char pattern[260];
    (void)snprintf(pattern, sizeof(pattern), "save\\%s\\*.sav",
        dispatch->player_selection.player_name);
    (void)pharaoh_file_list_load(&dispatch->saved_game_files, pattern, 0);
    pharaoh_file_list_sort(&dispatch->saved_game_files);
    dispatch->saved_game_first_visible = 0;
    dispatch->saved_game_selected_row = -1;
    dispatch->menu_load_return_window = 0x30;
    dispatch->refresh.current_window_id = 0x0d;
    dispatch->host->current_window_id = 0x0d;
    dispatch->host->graphics.redraw_requested = 1;
}

static void dispatch_main_menu_action(
    PharaohWindowsStartupDispatch *dispatch, int32_t action)
{
    PharaohMainMenuActionState state = { 0 };
    const PharaohMainMenuActionOperations operations = {
        dispatch, main_menu_stop_sound, NULL,
        main_menu_has_campaign_history, main_menu_confirm_campaign_restart,
        main_menu_open_map_selection, main_menu_open_saved_game_dialog,
        front_menu_open_player_selection
    };
    const PharaohMainMenuActionContext context = {
        &state, &operations, dispatch->host->campaign.saved_game_path
    };

    /* Loading an existing selected save requires the native save-state reader,
       which has no typed owner in restore/clean yet. */
    if (action == 0 &&
        dispatch->main_menu_window.selected_save_available != 0) return;

    state.window_state = &dispatch->main_menu_window;
    state.current_window_id = &dispatch->refresh.current_window_id;
    state.redraw_requested = &dispatch->host->graphics.redraw_requested;
    state.selection_flag_605a3c = &dispatch->menu_selection_flag_605a3c;
    state.selection_flag_6de515 = &dispatch->menu_selection_flag_6de515;
    state.main_menu_field_7027ac = &dispatch->menu_field_7027ac;
    state.main_menu_field_7027b0 = &dispatch->menu_field_7027b0;
    state.campaign_mode_6fdc98 = &dispatch->menu_campaign_mode_6fdc98;
    state.campaign_mode_e38ebc = &dispatch->menu_campaign_mode_e38ebc;
    state.map_mode_704dac = &dispatch->menu_map_mode_704dac;
    state.map_changed_704db0 = &dispatch->menu_map_changed_704db0;
    state.load_dialog_return_window = &dispatch->menu_load_return_window;
    pharaoh_main_menu_dispatch_action(&context, action);
    if (action == 4 && dispatch->player_profile_files.count == 0) {
        dispatch->refresh.current_window_id = 0x36;
    }
    dispatch->host->current_window_id = dispatch->refresh.current_window_id;
}

static void dispatch_front_menu_action(
    PharaohWindowsStartupDispatch *dispatch, int32_t action)
{
    PharaohGenericButtonInputState input_state = { 0 };
    PharaohFrontMenuActionState state = { 0 };
    const PharaohFrontMenuActionOperations operations = {
        dispatch,
        front_menu_open_player_selection,
        front_menu_show_support,
        front_menu_open_custom_maps,
        front_menu_open_high_scores,
        front_menu_request_exit
    };
    PharaohFrontMenuActionContext context;

    input_state.selected_primary_parameter = action;
    state.input_state = &input_state;
    state.action_active = &dispatch->front_menu_action_active;
    state.menu_action_reset = &dispatch->front_menu_action_reset;
    state.continue_campaign_pending = &dispatch->continue_campaign_pending;
    state.campaign_flow_mode = &dispatch->campaign_flow_mode;
    state.scenario_menu_changed = &dispatch->scenario_menu_changed;
    state.save_selection_mode = &dispatch->save_selection_mode;
    state.new_campaign_field_a = &dispatch->new_campaign_field_a;
    state.new_campaign_field_b = &dispatch->new_campaign_field_b;
    state.current_window_id = &dispatch->refresh.current_window_id;
    context.state = &state;
    context.operations = &operations;
    dispatch->front_menu_action_active = 1;
    pharaoh_front_menu_dispatch_action(&context);
    dispatch->host->current_window_id = dispatch->refresh.current_window_id;
}

int32_t pharaoh_windows_startup_dispatch_bind(
    PharaohWindowsStartupDispatch *dispatch, PharaohWindowsStartupHost *host)
{
    memset(dispatch, 0, sizeof(*dispatch));
    dispatch->host = host;
    pharaoh_file_list_initialize(&dispatch->player_profile_files);
    pharaoh_file_list_initialize(&dispatch->saved_game_files);
    dispatch->player_selection.profiles = &dispatch->player_profile_files;
    dispatch->player_selection.selected_profile = 0;
    pharaoh_file_list_initialize(&dispatch->custom_map_files);
    dispatch->custom_map_selection.files = &dispatch->custom_map_files;
    dispatch->custom_map_selection.selected_file = 0;
    pharaoh_high_scores_bind_campaign(&dispatch->high_scores,
        &host->campaign);
    pharaoh_windows_startup_host_bind_high_scores(host,
        &dispatch->high_scores_operations);
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
    dispatch->high_scores.origin_x = (host->graphics.render_width - 640) / 2;
    dispatch->high_scores.origin_y = (host->graphics.render_height - 480) / 2;
    dispatch->high_scores.title_font = 640;
    dispatch->high_scores.normal_font = 635;
    dispatch->high_scores.small_font = 634;
    dispatch->high_scores.empty_font = 636;
    dispatch->high_scores.panel_image_base =
        (int32_t)host->sprite_tags[284].image_index;
    dispatch->high_scores.redraw_request = &host->graphics.redraw_requested;
    dispatch->high_scores.current_window_id =
        &dispatch->refresh.current_window_id;
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
    pharaoh_file_list_destroy(&dispatch->player_profile_files);
    pharaoh_file_list_destroy(&dispatch->saved_game_files);
    pharaoh_file_list_destroy(&dispatch->custom_map_files);
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
        dispatch->mouse_button_state = 0xf1;
        if (dispatch->host != NULL &&
            dispatch->host->settings.sound_effects_enabled != 0) {
            pharaoh_windows_sound_button(&dispatch->host->sound,
                dispatch->host->settings.sound_effects_volume);
        }
    }
    if (message == WM_RBUTTONDOWN) {
        dispatch->right_press_active = 1;
        dispatch->right_press_window = dispatch->refresh.current_window_id;
        dispatch->mouse_button_state = 0xf3;
    }
    if (message == WM_LBUTTONUP) {
        if (dispatch->left_press_active != 0 &&
            dispatch->left_press_window == dispatch->refresh.current_window_id) {
            dispatch->input.mouse_left_went_up = 1;
            dispatch->input.video_input_flag_a = 1;
        }
        dispatch->left_press_active = 0;
        dispatch->mouse_button_state = 0xf2;
    }
    if (message == WM_RBUTTONUP) {
        if (dispatch->right_press_active != 0 &&
            dispatch->right_press_window == dispatch->refresh.current_window_id) {
            dispatch->input.mouse_right_went_up = 1;
            dispatch->input.video_input_flag_b = 1;
        }
        dispatch->right_press_active = 0;
        dispatch->mouse_button_state = 0xf0;
    }
}

void pharaoh_windows_startup_dispatch_escape(
    PharaohWindowsStartupDispatch *dispatch)
{
    if (dispatch == NULL || dispatch->refresh.current_window_id != 0 ||
        dispatch->exit_message_box_active != 0) {
        return;
    }
    front_menu_confirm_exit(dispatch);
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
        if (focused > 0 && dispatch->left_pressed_this_frame != 0 &&
                dispatch->left_press_window == 0) {
            dispatch_front_menu_action(dispatch, focused - 1);
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
    if (dispatch->refresh.current_window_id == 0x30) {
        const int32_t origin_x =
            (dispatch->host->graphics.render_width - 640) / 2;
        const int32_t origin_y =
            (dispatch->host->graphics.render_height - 480) / 2;
        PharaohGenericButtonFocusState focus_state = { 0 };
        const int32_t focused = pharaoh_generic_buttons_get_focus(
            origin_x, origin_y, pharaoh_game_menu_rects,
            PHARAOH_MENU_BUTTON_COUNT, dispatch->input.mouse_x,
            dispatch->input.mouse_y, &focus_state);
        if (focused != dispatch->main_menu_focused_button ||
            dispatch->host->graphics.redraw_requested != 0) {
            pharaoh_windows_startup_host_draw_main_menu(dispatch->host,
                &dispatch->main_menu_window, focused);
            dispatch->main_menu_focused_button = focused;
        }
        if (focused > 0 && dispatch->left_pressed_this_frame != 0 &&
            dispatch->left_press_window == 0x30) {
            dispatch_main_menu_action(dispatch, focused - 1);
        }
        dispatch->left_pressed_this_frame = 0;
        dispatch->input.mouse_left_went_up = 0;
        dispatch->input.mouse_right_went_up = 0;
        dispatch->input.video_input_flag_a = 0;
        dispatch->input.video_input_flag_b = 0;
        if (dispatch->refresh.current_window_id == 0x30) {
            pharaoh_graphics_refresh_screen(&dispatch->refresh,
                &dispatch->refresh_operations, 0);
            return;
        }
    }
    if (dispatch->refresh.current_window_id == 0x0d) {
        const int32_t origin_x =
            (dispatch->host->graphics.render_width - 640) / 2;
        const int32_t origin_y =
            (dispatch->host->graphics.render_height - 480) / 2;
        PharaohGenericButtonFocusState focus_state = { 0 };
        const int32_t focused = pharaoh_generic_buttons_get_focus(
            origin_x, origin_y, saved_game_row_rects, 12,
            dispatch->input.mouse_x, dispatch->input.mouse_y, &focus_state);

        if (dispatch->host->graphics.redraw_requested != 0) {
            pharaoh_windows_startup_host_draw_saved_game_list(dispatch->host,
                &dispatch->saved_game_files,
                dispatch->saved_game_first_visible,
                dispatch->saved_game_selected_row);
        }
        if (dispatch->input.mouse_right_went_up != 0 &&
            dispatch->right_press_window == 0x0d) {
            dispatch->refresh.current_window_id =
                (uint8_t)dispatch->menu_load_return_window;
            dispatch->host->current_window_id =
                dispatch->refresh.current_window_id;
            dispatch->host->graphics.redraw_requested = 1;
        } else if (dispatch->left_pressed_this_frame != 0 &&
            dispatch->left_press_window == 0x0d) {
            if (focused > 0) {
                dispatch->saved_game_selected_row =
                    dispatch->saved_game_first_visible + focused - 1;
                dispatch->host->graphics.redraw_requested = 1;
            } else if (dispatch->input.mouse_x >= origin_x + 208 &&
                dispatch->input.mouse_x < origin_x + 242 &&
                dispatch->saved_game_files.count > 12) {
                if (dispatch->input.mouse_y >= origin_y + 120 &&
                    dispatch->input.mouse_y < origin_y + 154 &&
                    dispatch->saved_game_first_visible > 0) {
                    --dispatch->saved_game_first_visible;
                    dispatch->host->graphics.redraw_requested = 1;
                } else if (dispatch->input.mouse_y >= origin_y + 300 &&
                    dispatch->input.mouse_y < origin_y + 334 &&
                    dispatch->saved_game_first_visible + 12 <
                        dispatch->saved_game_files.count) {
                    ++dispatch->saved_game_first_visible;
                    dispatch->host->graphics.redraw_requested = 1;
                }
            }
        }
        if (dispatch->left_press_active != 0 &&
            dispatch->left_press_window == 0x0d &&
            dispatch->saved_game_files.count > 12 &&
            dispatch->input.mouse_x >= origin_x + 0x1d0 &&
            dispatch->input.mouse_x <= origin_x + 0x1f0 &&
            dispatch->input.mouse_y >= origin_y + 0x91 &&
            dispatch->input.mouse_y <= origin_y + 300) {
            int32_t track = dispatch->input.mouse_y - origin_y - 0x91 - 25;
            const int32_t last = dispatch->saved_game_files.count - 12;
            if (track < 0) track = 0;
            if (track > 0x82) track = 0x82;
            dispatch->saved_game_first_visible =
                last * (track * 100 / 0x82) / 100;
            dispatch->host->graphics.redraw_requested = 1;
        }
        dispatch->left_pressed_this_frame = 0;
        dispatch->input.mouse_left_went_up = 0;
        dispatch->input.mouse_right_went_up = 0;
        dispatch->input.video_input_flag_a = 0;
        dispatch->input.video_input_flag_b = 0;
        if (dispatch->refresh.current_window_id == 0x0d) {
            pharaoh_graphics_refresh_screen(&dispatch->refresh,
                &dispatch->refresh_operations, 0);
            return;
        }
    }
    if (dispatch->refresh.current_window_id == 0x2f) {
        const int32_t origin_x =
            (dispatch->host->graphics.render_width - 640) / 2;
        const int32_t origin_y =
            (dispatch->host->graphics.render_height - 480) / 2;
        PharaohGenericButtonFocusState focus_state = { 0 };
        const int32_t focused = pharaoh_generic_buttons_get_focus(
            origin_x, origin_y, player_selection_rects, 16,
            dispatch->input.mouse_x, dispatch->input.mouse_y, &focus_state);
        if (focused != dispatch->player_selection_focused_button ||
            dispatch->host->graphics.redraw_requested != 0) {
            pharaoh_windows_startup_host_draw_player_selection(
                dispatch->host, &dispatch->player_selection, focused);
            dispatch->player_selection_focused_button = focused;
        }
        if (dispatch->left_press_active != 0 &&
            dispatch->left_press_window == 0x2f &&
            dispatch->input.mouse_x >= origin_x + 0x1d0 &&
            dispatch->input.mouse_x <= origin_x + 0x1f0 &&
            dispatch->input.mouse_y >= origin_y + 0x91 &&
            dispatch->input.mouse_y <= origin_y + 300 &&
            pharaoh_player_selection_drag_scrollbar(
                &dispatch->player_selection, dispatch->input.mouse_y,
                origin_y) != 0) {
            dispatch->host->graphics.redraw_requested = 1;
        }
        if (dispatch->left_pressed_this_frame != 0 &&
            dispatch->left_press_window == 0x2f) {
            if (dispatch->input.mouse_x >= origin_x + 464 &&
                dispatch->input.mouse_x < origin_x + 498 &&
                dispatch->input.mouse_y >= origin_y + 120 &&
                dispatch->input.mouse_y < origin_y + 154) {
                if (pharaoh_player_selection_scroll(
                        &dispatch->player_selection, 0) != 0) {
                    dispatch->host->graphics.redraw_requested = 1;
                }
            } else if (dispatch->input.mouse_x >= origin_x + 464 &&
                dispatch->input.mouse_x < origin_x + 498 &&
                dispatch->input.mouse_y >= origin_y + 300 &&
                dispatch->input.mouse_y < origin_y + 334) {
                if (pharaoh_player_selection_scroll(
                        &dispatch->player_selection, 1) != 0) {
                    dispatch->host->graphics.redraw_requested = 1;
                }
            } else if (focused >= 1 && focused <= 12) {
                if (pharaoh_player_selection_select_visible(
                        &dispatch->player_selection, focused - 1) != 0) {
                    char profile_path[64];
                    (void)snprintf(profile_path, sizeof(profile_path),
                        "save\\%s.dat",
                        dispatch->player_selection.player_name);
                    (void)pharaoh_campaign_profile_load(
                        &dispatch->host->files, &dispatch->host->campaign,
                        dispatch->host->campaign_profile_suffix, profile_path);
                    /* Native 0x00516AB0 selects and loads this profile but
                       leaves the player-selection window active. */
                    dispatch->host->graphics.redraw_requested = 1;
                }
            } else if (focused == 15 &&
                dispatch->player_selection.player_name[0] != '\0') {
                /* Native 0x00516990 opens the main menu for the selected
                   player; its empty-name dialog is still unbound. */
                open_selected_player_menu(dispatch);
                dispatch->host->graphics.redraw_requested = 1;
                dispatch->player_selection_focused_button = 0;
            } else if (focused == 16) {
                /* Native 0x00402BB0 returns to window zero. */
                dispatch->refresh.current_window_id = 0;
                dispatch->host->current_window_id = 0;
                dispatch->host->graphics.redraw_requested = 1;
                dispatch->menu_focused_button = 0;
            }
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
    if (dispatch->refresh.current_window_id == 0x3d) {
        const int32_t origin_x =
            (dispatch->host->graphics.render_width - 640) / 2;
        const int32_t origin_y =
            (dispatch->host->graphics.render_height - 480) / 2;
        if (dispatch->left_pressed_this_frame != 0 &&
            dispatch->left_press_window == 0x3d &&
            dispatch->input.mouse_x >= origin_x + 282 &&
            dispatch->input.mouse_x < origin_x + 316) {
            if (dispatch->input.mouse_y >= origin_y + 215 &&
                dispatch->input.mouse_y < origin_y + 249) {
                if (pharaoh_custom_map_selection_scroll(
                        &dispatch->custom_map_selection, 0) != 0) {
                    dispatch->host->graphics.redraw_requested = 1;
                }
            } else if (dispatch->input.mouse_y >= origin_y + 435 &&
                dispatch->input.mouse_y < origin_y + 469) {
                if (pharaoh_custom_map_selection_scroll(
                        &dispatch->custom_map_selection, 1) != 0) {
                    dispatch->host->graphics.redraw_requested = 1;
                }
            }
        }
        if (dispatch->left_press_active != 0 &&
            dispatch->left_press_window == 0x3d &&
            pharaoh_custom_map_selection_drag_scrollbar(
                &dispatch->custom_map_selection, dispatch->input.mouse_x,
                dispatch->input.mouse_y, origin_x, origin_y) != 0) {
            dispatch->host->graphics.redraw_requested = 1;
        }
        if (dispatch->left_pressed_this_frame != 0 &&
            dispatch->left_press_window == 0x3d &&
            dispatch->input.mouse_x >= origin_x + 335 &&
            dispatch->input.mouse_x < origin_x + 362 &&
            dispatch->input.mouse_y >= origin_y + 440 &&
            dispatch->input.mouse_y < origin_y + 467) {
            /* State 0x3D's native back callback calls 0x004CB6A0. */
            open_selected_player_menu(dispatch);
            dispatch->host->graphics.redraw_requested = 1;
        }
        if (dispatch->refresh.current_window_id == 0x3d &&
            dispatch->host->graphics.redraw_requested != 0) {
            pharaoh_windows_startup_host_draw_custom_map_list(dispatch->host,
                &dispatch->custom_map_selection);
        }
        dispatch->host->current_window_id = dispatch->refresh.current_window_id;
        dispatch->left_pressed_this_frame = 0;
        dispatch->input.mouse_left_went_up = 0;
        dispatch->input.mouse_right_went_up = 0;
        dispatch->input.video_input_flag_a = 0;
        dispatch->input.video_input_flag_b = 0;
        if (dispatch->refresh.current_window_id == 0x3d) {
            pharaoh_graphics_refresh_screen(&dispatch->refresh,
                &dispatch->refresh_operations, 0);
            return;
        }
    }
    if (dispatch->refresh.current_window_id == 0x53) {
        const PharaohHighScoresInput input = {
            dispatch->input.mouse_x,
            dispatch->input.mouse_y,
            dispatch->left_pressed_this_frame,
            dispatch->left_press_active,
            dispatch->input.mouse_right_went_up
        };
        if (dispatch->host->graphics.redraw_requested != 0) {
            pharaoh_high_scores_draw_background(&dispatch->high_scores,
                &dispatch->high_scores_operations);
        }
        (void)pharaoh_high_scores_handle_input(&dispatch->high_scores, &input);
        dispatch->host->current_window_id = dispatch->refresh.current_window_id;
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
