#include "pharaoh/platform/windows_mission_runtime.h"
#include "pharaoh/platform/windows_startup_dispatch.h"
#include "pharaoh/platform/windows_city_ui.h"
#include "pharaoh/ui/campaign_menu.h"
#include "pharaoh/ui/front_menu_action.h"
#include "pharaoh/ui/main_menu.h"
#include "pharaoh/ui/map_selection_invasions.h"
#include "pharaoh/ui/load_save_dialog.h"
#include "pharaoh/runtime/file_extension_operations.h"

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

enum {
    FAMILY_MESSAGE_NONE = 0,
    FAMILY_MESSAGE_DELETE = 1,
    FAMILY_MESSAGE_DUPLICATE = 2
};

static void load_selected_family(PharaohWindowsStartupDispatch *dispatch);

static void map_selection_adjust_difficulty_ui(
    PharaohWindowsStartupDispatch *dispatch, int32_t decrement)
{
    PharaohWindowsStartupHost *const host = dispatch->host;
    int32_t difficulty = host->settings.difficulty + (decrement ? -1 : 1);
    if (difficulty < 0) difficulty = 0;
    else if (difficulty > 4) difficulty = 4;
    host->settings.difficulty = difficulty;
    host->building_model_context.difficulty =
        (PharaohModelDifficulty)difficulty;
    host->figure_model_context.difficulty =
        (PharaohModelDifficulty)difficulty;
    host->tax_model_context.difficulty =
        (PharaohModelDifficulty)difficulty;
    (void)pharaoh_building_model_load(&host->building_model_context);
    (void)pharaoh_figure_model_load(&host->figure_model_context);
    (void)pharaoh_tax_sentiment_model_load(&host->tax_model_context);
    host->graphics.redraw_requested = 1;
}

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

static int32_t map_selection_load_custom_preview(void *user_data,
    const char *path, int32_t mode)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    int32_t result;
    (void)mode;
    result = pharaoh_scenario_preview_load_map(
        &dispatch->custom_map_selection.preview, path);
    if (result == 0) {
        dispatch->custom_map_selection.preview_invasion_count =
            pharaoh_map_selection_count_invasions(
                (PharaohMapSelectionEventList *)(void *)
                    dispatch->custom_map_selection.preview.events,
                &dispatch->custom_map_selection.preview_current_event,
                &dispatch->custom_map_selection.preview_invasion_search);
    }
    return result;
}

static int32_t map_selection_load_campaign_preview(void *user_data,
    int32_t package_index, int32_t skip_initialize)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    int32_t result;
    (void)skip_initialize;
    if (package_index != 0) return 0;
    result = pharaoh_scenario_preview_load_campaign_mission(
        &dispatch->custom_map_selection.preview, "mission1.pak",
        dispatch->custom_map_selection.warning_text_id);
    if (result != 0) {
        dispatch->custom_map_selection.preview_invasion_count =
            pharaoh_map_selection_count_invasions(
                (PharaohMapSelectionEventList *)(void *)
                    dispatch->custom_map_selection.preview.events,
                &dispatch->custom_map_selection.preview_current_event,
                &dispatch->custom_map_selection.preview_invasion_search);
    }
    return result;
}

static int32_t map_selection_activate_row(
    PharaohWindowsStartupDispatch *dispatch, int32_t visible_row)
{
    const PharaohMapSelectionOperations operations = {
        dispatch,
        map_selection_load_custom_preview,
        map_selection_load_campaign_preview,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    };
    return pharaoh_map_selection_activate(&dispatch->custom_map_selection,
        visible_row, &operations,
        &dispatch->host->graphics.redraw_requested);
}

static void front_menu_open_player_selection(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    const int32_t previous_profile =
        dispatch->player_selection.selected_profile;
    const int32_t profile_count = pharaoh_player_selection_load_profiles(
        &dispatch->player_selection, previous_profile);
    dispatch->refresh.current_window_id = profile_count == 0 ? 0x36 : 0x2f;
    dispatch->host->current_window_id = dispatch->refresh.current_window_id;
    dispatch->ui_feedback_countdown = 0;
    if (profile_count != 0 &&
        dispatch->player_selection.selected_profile >= 0) {
        load_selected_family(dispatch);
    } else if (profile_count == 0) {
        PharaohTextInputState *const input =
            &dispatch->host->text_input;
        PharaohTextInputField *const field = &input->fields[1];

        /* Native 0x00516BB0 enters the new-family editor with field 1
           bound to the 31-byte player-name buffer. */
        field->text_buffer = dispatch->player_selection.player_name;
        field->max_length = 0x1f;
        input->return_pressed = 0;
        pharaoh_text_input_activate(input, 1);
        pharaoh_keyboard_home(input);
        pharaoh_keyboard_end(input);
        dispatch->player_selection_return_window = 0x2f;
    }
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
    (void)map_selection_activate_row(dispatch,
        dispatch->custom_map_selection.selected_file);
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
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    MSG message;
    const int message_result = GetMessageA(&message, NULL, 0, 0);
    if (message_result <= 0) {
        /* The nested confirmation loop consumed WM_QUIT.  Hand it back to
           WinMain's loop; otherwise the destroyed window leaves audio alive. */
        if (message_result == 0) {
            PostQuitMessage((int)message.wParam);
        }
        dispatch->host->shutdown_requested = 1;
        return 0;
    }
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

static void show_family_message(PharaohWindowsStartupDispatch *dispatch,
    int32_t first_text_entry, uint8_t native_button_count, uint8_t action)
{
    dispatch->family_message_previous_window =
        dispatch->refresh.current_window_id;
    dispatch->family_message_text_entry = first_text_entry;
    dispatch->family_message_button_count = native_button_count;
    dispatch->family_message_action = action;
    dispatch->family_message_focused_button = 0;
    dispatch->family_message_previous_profile =
        dispatch->player_selection.selected_profile;
    dispatch->refresh.current_window_id = 0x3f;
    dispatch->host->current_window_id = 0x3f;
    dispatch->host->graphics.redraw_requested = 1;
}

static void load_selected_family(PharaohWindowsStartupDispatch *dispatch)
{
    char directory[MAX_PATH];
    char profile[MAX_PATH];
    if (pharaoh_player_profile_paths(
            dispatch->player_selection.player_name,
            directory, sizeof(directory), profile, sizeof(profile)) != 0) {
        (void)pharaoh_campaign_profile_load(&dispatch->host->files,
            &dispatch->host->campaign,
            dispatch->host->campaign_profile_suffix, profile);
    }
}

static void reload_family_registry(PharaohWindowsStartupDispatch *dispatch,
    int32_t previous_profile)
{
    (void)pharaoh_player_selection_load_profiles(
        &dispatch->player_selection, previous_profile);
    if (dispatch->player_selection.selected_profile >= 0) {
        load_selected_family(dispatch);
    }
    dispatch->refresh.current_window_id = 0x2f;
    dispatch->host->current_window_id = 0x2f;
    dispatch->host->graphics.redraw_requested = 1;
    dispatch->player_selection_focused_button = 0;
}

static void begin_new_family(PharaohWindowsStartupDispatch *dispatch)
{
    memset(dispatch->player_selection.player_name, 0,
        sizeof(dispatch->player_selection.player_name));
    dispatch->refresh.current_window_id = 0x36;
    dispatch->host->current_window_id = 0x36;
    dispatch->host->graphics.redraw_requested = 1;
    dispatch->player_selection_focused_button = 0;
}

static void accept_new_family(PharaohWindowsStartupDispatch *dispatch)
{
    PharaohPlayerSelectionState *const selection =
        &dispatch->player_selection;
    char saved_name[PHARAOH_PLAYER_NAME_CAPACITY];
    char directory[MAX_PATH];
    char profile[MAX_PATH];
    DWORD attributes;
    int32_t selected;

    pharaoh_player_name_trim(selection->player_name);
    if (selection->player_name[0] == '\0') {
        reload_family_registry(dispatch, selection->selected_profile);
        if (selection->profiles->count == 0) begin_new_family(dispatch);
        return;
    }
    if (pharaoh_player_profile_paths(selection->player_name,
            directory, sizeof(directory), profile, sizeof(profile)) == 0) {
        return;
    }
    attributes = GetFileAttributesA(profile);
    if (attributes != INVALID_FILE_ATTRIBUTES) {
        show_family_message(dispatch, 0x5c, 1, FAMILY_MESSAGE_DUPLICATE);
        return;
    }

    (void)pharaoh_campaign_load(&dispatch->host->campaign, "campaign.txt",
        &dispatch->host->campaign_file_operations);
    (void)pharaoh_campaign_profile_save(&dispatch->host->campaign,
        dispatch->host->campaign_profile_suffix, profile);
    (void)CreateDirectoryA(directory, NULL);
    memcpy(saved_name, selection->player_name, sizeof(saved_name));
    (void)pharaoh_player_selection_load_profiles(selection,
        selection->selected_profile);
    selected = pharaoh_player_selection_find_profile(selection, saved_name);
    memcpy(selection->player_name, saved_name, sizeof(saved_name));
    (void)pharaoh_player_selection_load_profiles(selection, selected);
    load_selected_family(dispatch);
    open_selected_player_menu(dispatch);
    dispatch->host->graphics.redraw_requested = 1;
}

static void delete_selected_family(PharaohWindowsStartupDispatch *dispatch)
{
    PharaohPlayerSelectionState *const selection =
        &dispatch->player_selection;
    PharaohFileList saves;
    char directory[MAX_PATH];
    char profile[MAX_PATH];
    char pattern[MAX_PATH];
    char path[MAX_PATH];
    int32_t previous = selection->selected_profile;

    if (selection->player_name[0] == '\0' ||
        pharaoh_player_profile_paths(selection->player_name,
            directory, sizeof(directory), profile, sizeof(profile)) == 0) {
        return;
    }
    (void)DeleteFileA(profile);
    pharaoh_file_list_initialize(&saves);
    if (snprintf(pattern, sizeof(pattern), "%s\\*.sav", directory) > 0) {
        (void)pharaoh_file_list_load(&saves, pattern, 0);
    }
    for (int32_t index = 0; index < saves.count; ++index) {
        const char *const name = pharaoh_file_list_name(&saves, index);
        if (name != NULL && snprintf(path, sizeof(path), "%s\\%s",
                directory, name) > 0) {
            (void)DeleteFileA(path);
        }
    }
    pharaoh_file_list_destroy(&saves);
    (void)RemoveDirectoryA(directory);
    reload_family_registry(dispatch, previous);
    if (selection->profiles->count == 0) begin_new_family(dispatch);
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
    const char *const title = exit_box_select_text(dispatch, 5, 141);
    const char *const body = exit_box_select_text(dispatch, 5, 142);
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
    (void)map_selection_activate_row(dispatch,
        dispatch->custom_map_selection.selected_file);
    dispatch->refresh.current_window_id = 0x3d;
    dispatch->host->current_window_id = 0x3d;
    dispatch->map_selection_focused_button = 0;
    dispatch->map_selection_hovered_row = -1;
    dispatch->host->graphics.redraw_requested = 1;
}

static void map_selection_back(PharaohWindowsStartupDispatch *dispatch)
{
    const PharaohMapSelectionBackOperations operations = {
        dispatch, open_selected_player_menu
    };
    pharaoh_map_selection_back(dispatch->menu_selection_flag_605a3c,
        &dispatch->refresh.current_window_id,
        &dispatch->host->graphics.redraw_requested, &operations);
    dispatch->host->current_window_id =
        dispatch->refresh.current_window_id;
}

static void campaign_menu_play_period_intro(void *user_data, uint32_t period)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    PharaohWindowsStartupHost *const host = dispatch->host;
    PharaohCampaignPeriodVideo video;

    if (pharaoh_campaign_menu_period_video(period, host->settings.cpu,
            host->graphics.screen_width, host->graphics.screen_height, &video)) {
        host->current_window_id = dispatch->refresh.current_window_id;
        (void)pharaoh_windows_startup_host_play_movie(host,
            video.path, video.x, video.y);
        dispatch->refresh.current_window_id = host->current_window_id;
    }
}

static void campaign_menu_start_campaign(void *user_data, int8_t period)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    PharaohCampaignStartContext context =
        dispatch->host->settings_context.campaign;
    context.settings = &dispatch->host->settings;
    pharaoh_settings_start_campaign(&context, period);
}

static void campaign_menu_begin_briefing(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    const PharaohTimedWindowTransitionStartContext transition = {
        &dispatch->timed_transition,
        &dispatch->timed_transition_value,
        &dispatch->timed_transition_next_window
    };
    const PharaohCampaignBriefingContext context = {
        &dispatch->host->campaign_index,
        &dispatch->host->settings,
        (uint32_t)dispatch->host->settings.is_custom_scenario,
        transition,
        &dispatch->briefing
    };
    pharaoh_windows_mission_runtime_destroy(dispatch->mission_runtime);
    dispatch->mission_runtime = NULL;
    pharaoh_campaign_begin_briefing(&context);
    dispatch->host->current_window_id = dispatch->refresh.current_window_id;
}

static PharaohCampaignMenuState campaign_menu_state(
    PharaohWindowsStartupDispatch *dispatch)
{
    PharaohCampaignMenuState state = {
        &dispatch->refresh.current_window_id,
        &dispatch->host->graphics.redraw_requested,
        &dispatch->menu_field_7027ac,
        &dispatch->menu_field_7027b0,
        &dispatch->menu_campaign_mode_6fdc98,
        &dispatch->menu_map_changed_704db0,
        &dispatch->menu_selection_flag_605a3c,
        &dispatch->menu_selection_flag_6de515,
        &dispatch->menu_map_mode_704dac,
        &dispatch->campaign_started,
        &dispatch->host->settings.mission_id,
        &dispatch->campaign_selection_kind,
        (uint32_t *)(void *)&dispatch->host->settings.is_custom_scenario
    };
    return state;
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
    state.campaign_mode_e38ebc = (uint32_t *)(void *)&dispatch->host->settings.is_custom_scenario;
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
    state.map_selection_returns_to_front_menu =
        &dispatch->menu_selection_flag_605a3c;
    state.save_selection_mode = &dispatch->save_selection_mode;
    state.map_selection_campaign_mode = &dispatch->menu_map_changed_704db0;
    state.map_selection_mode = &dispatch->menu_map_mode_704dac;
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
    host->native_force_refresh = &dispatch->refresh.force_refresh;
    /* Native 00540B50 field1 binding, using the actual loaded small-font ID. */
    pharaoh_text_input_field_initialize(&host->text_input, 1,
        dispatch->player_selection.player_name, 31, 104, 1,
        host->sprite_tags[634].image_index);
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
    dispatch->timed_transition.advance_input =
        &dispatch->input.mouse_right_went_up;
    dispatch->timed_transition.current_window_id =
        &dispatch->refresh.current_window_id;
    dispatch->timed_transition.next_window_id =
        &dispatch->timed_transition_next_window;
    dispatch->timed_transition.redraw_requested =
        &host->graphics.redraw_requested;
    dispatch->timed_transition.clock_user_data = dispatch;
    dispatch->timed_transition.get_tick_count = frame_clock;
    dispatch->transition.timed_transition = &dispatch->timed_transition;
    dispatch->video.redraw_requested = &host->graphics.redraw_requested;
    dispatch->high_scores.origin_x = (host->graphics.render_width - 640) / 2;
    dispatch->high_scores.origin_y = (host->graphics.render_height - 480) / 2;
    dispatch->high_scores.title_font = 640;
    dispatch->high_scores.normal_font = 635;
    dispatch->high_scores.small_font = 634;
    dispatch->high_scores.empty_font = 636;
    dispatch->high_scores.panel_image_base =
        (int32_t)host->sprite_tags[284].image_index;
    dispatch->high_scores.scroll_button_image_base =
        (int32_t)host->sprite_tags[365].image_index;
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
    pharaoh_windows_mission_runtime_destroy(dispatch->mission_runtime);
    dispatch->mission_runtime = NULL;
    pharaoh_windows_city_ui_release(dispatch->host);
    pharaoh_file_list_destroy(&dispatch->player_profile_files);
    pharaoh_file_list_destroy(&dispatch->saved_game_files);
    pharaoh_file_list_destroy(&dispatch->custom_map_files);
    pharaoh_windows_smacker_close_library(&dispatch->smacker);
}

/* Native 0x00516AB0 selects a profile and loads its campaign profile. */
static void load_clicked_player_profile(
    PharaohWindowsStartupDispatch *dispatch, int32_t visible_row)
{
    PharaohPlayerSelectionState *const selection =
        &dispatch->player_selection;
    const int32_t selected = selection->first_visible_profile + visible_row;
    char profile_path[64];

    if (visible_row < 0 ||
        visible_row >= PHARAOH_PLAYER_SELECTION_VISIBLE_ROWS ||
        selected < 0 || selected >= selection->profiles->count ||
        pharaoh_file_list_name(selection->profiles, selected) == NULL) {
        return;
    }
    if (pharaoh_player_selection_select_visible(selection, visible_row) == 0)
        return;

    pharaoh_file_remove_extension(selection->player_name);
    pharaoh_player_record_path(profile_path, selection->player_name);
    (void)pharaoh_campaign_profile_load(&dispatch->host->files,
        &dispatch->host->campaign, dispatch->host->campaign_profile_suffix,
        profile_path);
    dispatch->host->graphics.redraw_requested = 1;
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
        if (dispatch->right_press_window == 0x2f &&
            dispatch->refresh.current_window_id == 0x2f) {
            dispatch->refresh.current_window_id = 0;
            dispatch->host->current_window_id = 0;
            dispatch->host->graphics.redraw_requested = 1;
            dispatch->menu_focused_button = 0;
            dispatch->input.mouse_right_went_up = 0;
            dispatch->input.video_input_flag_b = 0;
        }
    }
}

void pharaoh_windows_startup_dispatch_character(
    PharaohWindowsStartupDispatch *dispatch, WPARAM character)
{
    char *const name = dispatch->player_selection.player_name;
    size_t length;

    if (dispatch->refresh.current_window_id != 0x36) return;
    if (character == '\r') {
        accept_new_family(dispatch);
        return;
    }
    length = strlen(name);
    if (character == '\b') {
        if (length != 0) name[length - 1] = '\0';
    } else if (character >= 0x20 && character <= 0x7e &&
            length + 1 < sizeof(dispatch->player_selection.player_name)) {
        name[length] = (char)character;
        name[length + 1] = '\0';
    } else {
        return;
    }
    dispatch->host->graphics.redraw_requested = 1;
}

void pharaoh_windows_startup_dispatch_escape(
    PharaohWindowsStartupDispatch *dispatch)
{
    if (dispatch == NULL || dispatch->exit_message_box_active != 0) return;
    /* Native window procedure handles VK_ESCAPE before window dispatch and
       always opens the group 5 entries 0/1 quit confirmation. */
    front_menu_confirm_exit(dispatch);
}

void pharaoh_windows_startup_dispatch_frame(void *user_data)
{
    PharaohWindowsStartupDispatch *const dispatch = user_data;
    PharaohSmackerWindowVideoState *const video = dispatch->transition.video;
    const uint8_t previous_window = dispatch->refresh.current_window_id;
    const int32_t previous_video_target = video->return_window_id;
    if (dispatch->refresh.current_window_id == 1 &&
        dispatch->mission_runtime != NULL) {
        if(pharaoh_windows_mission_runtime_game_frame(dispatch->mission_runtime,
            dispatch->input.mouse_x,dispatch->input.mouse_y,
            dispatch->left_press_active,(uint8_t)(dispatch->mouse_button_state==0xf3))!=0) {
            pharaoh_graphics_refresh_screen(&dispatch->refresh,
                &dispatch->refresh_operations,0);
        }
        dispatch->host->current_window_id = 1;
        dispatch->left_pressed_this_frame = 0;
        dispatch->input.mouse_left_went_up = 0;
        dispatch->input.mouse_right_went_up = 0;
        return;
    }
    /* Window-zero drawing and hover; the full menu action chain remains a
       separate owner and is not replaced with invented campaign behavior. */
    if (dispatch->refresh.current_window_id == 0x35) {
        if (dispatch->mission_runtime == NULL) {
            dispatch->mission_runtime = pharaoh_windows_mission_runtime_create(
                dispatch->host, &dispatch->briefing,
                &dispatch->refresh.current_window_id);
        }
        if (dispatch->mission_runtime != NULL) {
            pharaoh_windows_mission_runtime_briefing(dispatch->mission_runtime,
                dispatch->input.mouse_x, dispatch->input.mouse_y,
                dispatch->left_press_window == 0x35 ? dispatch->left_pressed_this_frame : 0,
                dispatch->mouse_button_state == 0xf3,
                dispatch->left_press_active);
        }
        dispatch->host->current_window_id = dispatch->refresh.current_window_id;
        dispatch->left_pressed_this_frame = 0;
        dispatch->input.mouse_left_went_up = 0;
        dispatch->input.mouse_right_went_up = 0;
        dispatch->input.video_input_flag_a = 0;
        dispatch->input.video_input_flag_b = 0;
        if (dispatch->refresh.current_window_id == 0x35) {
            pharaoh_graphics_refresh_screen(&dispatch->refresh,
                &dispatch->refresh_operations, 0);
        }
        return;
    }
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
    if (dispatch->refresh.current_window_id == 0x3f) {
        const int32_t origin_x =
            (dispatch->host->graphics.render_width - 640) / 2;
        const int32_t origin_y =
            (dispatch->host->graphics.render_height - 480) / 2;
        int32_t focused = 0;
        int32_t accepted = 0;

        if (dispatch->input.mouse_x >= origin_x + 336 &&
            dispatch->input.mouse_x < origin_x + 370 &&
            dispatch->input.mouse_y >= origin_y + 180 &&
            dispatch->input.mouse_y < origin_y + 214) {
            focused = 1;
        } else if (dispatch->family_message_button_count == 2 &&
            dispatch->input.mouse_x >= origin_x + 272 &&
            dispatch->input.mouse_x < origin_x + 306 &&
            dispatch->input.mouse_y >= origin_y + 180 &&
            dispatch->input.mouse_y < origin_y + 214) {
            focused = 2;
        }
        pharaoh_windows_startup_host_draw_family_message(dispatch->host,
            dispatch->family_message_text_entry,
            dispatch->family_message_button_count, focused);
        if (dispatch->left_pressed_this_frame != 0 &&
            dispatch->left_press_window == 0x3f && focused != 0) {
            const uint8_t action = dispatch->family_message_action;
            accepted = focused == 1;
            dispatch->refresh.current_window_id =
                dispatch->family_message_previous_window;
            dispatch->host->current_window_id =
                dispatch->family_message_previous_window;
            if (action == FAMILY_MESSAGE_DELETE) {
                if (accepted != 0) delete_selected_family(dispatch);
                else reload_family_registry(dispatch,
                    dispatch->family_message_previous_profile);
            } else if (action == FAMILY_MESSAGE_DUPLICATE) {
                begin_new_family(dispatch);
            }
            dispatch->host->graphics.redraw_requested = 1;
        }
        dispatch->family_message_focused_button = (uint8_t)focused;
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
        if (dispatch->input.mouse_right_went_up != 0 &&
            dispatch->right_press_window == 0x30) {
            front_menu_open_player_selection(dispatch);
            dispatch->input.mouse_right_went_up = 0;
            dispatch->input.video_input_flag_b = 0;
            dispatch->host->graphics.redraw_requested = 1;
            return;
        }
        const int32_t focused = pharaoh_generic_buttons_get_focus(
            origin_x, origin_y, pharaoh_game_menu_rects,
            PHARAOH_MENU_BUTTON_COUNT, dispatch->input.mouse_x,
            dispatch->input.mouse_y, &focus_state);
        if (focused != dispatch->main_menu_focused_button ||
            dispatch->host->graphics.redraw_requested != 0) {
            pharaoh_windows_startup_host_draw_main_menu(dispatch->host,
                &dispatch->main_menu_window,
                dispatch->player_selection.player_name, focused);
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
                const int32_t selected =
                    pharaoh_load_dialog_select_visible(
                        &dispatch->saved_game_files,
                        dispatch->saved_game_first_visible, focused - 1);
                if (selected >= 0) {
                    dispatch->saved_game_selected_row = selected;
                    dispatch->saved_game_error_countdown = 0;
                    dispatch->host->graphics.redraw_requested = 1;
                }
            } else if (dispatch->input.mouse_x >= origin_x + 464 &&
                dispatch->input.mouse_x < origin_x + 498 &&
                dispatch->saved_game_files.count > 12) {
                if (dispatch->input.mouse_y >= origin_y + 120 &&
                    dispatch->input.mouse_y < origin_y + 154 &&
                    pharaoh_load_dialog_scroll(0,
                        dispatch->saved_game_files.count,
                        &dispatch->saved_game_first_visible,
                        &dispatch->saved_game_error_countdown) != 0) {
                    dispatch->host->graphics.redraw_requested = 1;
                } else if (dispatch->input.mouse_y >= origin_y + 300 &&
                    dispatch->input.mouse_y < origin_y + 334 &&
                    pharaoh_load_dialog_scroll(1,
                        dispatch->saved_game_files.count,
                        &dispatch->saved_game_first_visible,
                        &dispatch->saved_game_error_countdown) != 0) {
                    dispatch->host->graphics.redraw_requested = 1;
                }
            } else if (dispatch->input.mouse_x >= origin_x + 344 &&
                dispatch->input.mouse_x < origin_x + 378 &&
                dispatch->input.mouse_y >= origin_y + 335 &&
                dispatch->input.mouse_y < origin_y + 369) {
                dispatch->refresh.current_window_id =
                    (uint8_t)dispatch->menu_load_return_window;
                dispatch->host->current_window_id =
                    dispatch->refresh.current_window_id;
                dispatch->host->graphics.redraw_requested = 1;
            }
        }
        if (dispatch->left_press_window == 0x0d &&
            pharaoh_load_dialog_drag_scrollbar(
                dispatch->left_press_active, dispatch->input.mouse_x,
                dispatch->input.mouse_y, origin_x, origin_y,
                dispatch->saved_game_files.count,
                &dispatch->saved_game_first_visible) != 0) {
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
    if (dispatch->refresh.current_window_id == 0x36) {
        const int32_t origin_x =
            (dispatch->host->graphics.render_width - 640) / 2;
        const int32_t origin_y =
            (dispatch->host->graphics.render_height - 480) / 2;
        const int32_t list_button_focused =
            dispatch->input.mouse_x >= origin_x + 352 &&
            dispatch->input.mouse_x < origin_x + 377 &&
            dispatch->input.mouse_y >= origin_y + 246 &&
            dispatch->input.mouse_y < origin_y + 271;
        const int32_t accept_button_focused =
            dispatch->input.mouse_x >= origin_x + 464 &&
            dispatch->input.mouse_x < origin_x + 498 &&
            dispatch->input.mouse_y >= origin_y + 249 &&
            dispatch->input.mouse_y < origin_y + 283;
        const int32_t focused_button = list_button_focused != 0 ? 1 :
            accept_button_focused != 0 ? 2 : 0;

        if (dispatch->input.mouse_right_went_up != 0 &&
            dispatch->right_press_window == 0x36) {
            front_menu_open_player_selection(dispatch);
        } else if (dispatch->left_pressed_this_frame != 0 &&
            dispatch->left_press_window == 0x36) {
            if (list_button_focused != 0) {
                pharaoh_player_name_choices_open(
                    &dispatch->refresh.current_window_id,
                    &dispatch->host->graphics.redraw_requested);
                dispatch->host->current_window_id =
                    dispatch->refresh.current_window_id;
            } else if (accept_button_focused != 0) {
                accept_new_family(dispatch);
            }
        }

        if (dispatch->refresh.current_window_id != 0x36) {
            dispatch->left_pressed_this_frame = 0;
            dispatch->input.mouse_left_went_up = 0;
            dispatch->input.mouse_right_went_up = 0;
            return;
        }

        /* Native foreground 0x00520B90 runs each frame so 0x00537140 can
           animate the edit caret. */
        pharaoh_windows_startup_host_draw_player_name_input(
            dispatch->host, &dispatch->player_selection,
            focused_button);
        dispatch->player_selection_focused_button = focused_button;
        dispatch->left_pressed_this_frame = 0;
        dispatch->input.mouse_left_went_up = 0;
        dispatch->input.mouse_right_went_up = 0;
        dispatch->input.video_input_flag_a = 0;
        dispatch->input.video_input_flag_b = 0;
        pharaoh_graphics_refresh_screen(&dispatch->refresh,
            &dispatch->refresh_operations, 0);
        return;
    }
    if (dispatch->refresh.current_window_id == 0x17) {
        static const int16_t period_buttons[9][4] = {
            { 20, 291, 164, 316 }, { 20, 321, 164, 346 },
            { 172, 261, 316, 286 }, { 172, 291, 316, 316 },
            { 172, 321, 316, 346 }, { 20, 391, 164, 416 },
            { 20, 421, 164, 446 }, { 172, 391, 316, 416 },
            { 172, 421, 316, 446 }
        };
        const int32_t origin_x =
            (dispatch->host->graphics.render_width - 640) / 2;
        const int32_t origin_y =
            (dispatch->host->graphics.render_height - 480) / 2;
        const int32_t selected = (int32_t)dispatch->menu_field_7027ac;
        const int32_t latest = (int32_t)dispatch->menu_field_7027b0;
        const int32_t mode = (int32_t)dispatch->menu_campaign_mode_6fdc98;
        const int32_t image_button_count =
            pharaoh_campaign_menu_image_button_count(
                (uint32_t)mode, (uint32_t)selected, (uint32_t)latest);
        const int32_t selected_enabled =
            pharaoh_campaign_menu_period_valid(selected) &&
            image_button_count != 0;
        int32_t focused = 0;

        for (int32_t index = 0; index < 9; ++index) {
            if (dispatch->input.mouse_x >= origin_x + period_buttons[index][0] &&
                dispatch->input.mouse_x < origin_x + period_buttons[index][2] &&
                dispatch->input.mouse_y >= origin_y + period_buttons[index][1] &&
                dispatch->input.mouse_y < origin_y + period_buttons[index][3]) {
                focused = index + 1;
                break;
            }
        }
        if (mode == 1 &&
            dispatch->input.mouse_x >= origin_x + 32 &&
            dispatch->input.mouse_x < origin_x + 192 &&
            dispatch->input.mouse_y >= origin_y + 208 &&
            dispatch->input.mouse_y < origin_y + 233) {
            focused = 10;
        } else if (mode == 1 &&
            dispatch->input.mouse_x >= origin_x + 195 &&
            dispatch->input.mouse_x < origin_x + 288 &&
            dispatch->input.mouse_y >= origin_y + 208 &&
            dispatch->input.mouse_y < origin_y + 233) {
            focused = 11;
        } else if (selected_enabled &&
            dispatch->input.mouse_x >= origin_x + 600 &&
            dispatch->input.mouse_x < origin_x + 627 &&
            dispatch->input.mouse_y >= origin_y + 440 &&
            dispatch->input.mouse_y < origin_y + 467) {
            focused = 12;
        } else if (selected_enabled && mode == 1 &&
            dispatch->input.mouse_x >= origin_x + 335 &&
            dispatch->input.mouse_x < origin_x + 362 &&
            dispatch->input.mouse_y >= origin_y + 440 &&
            dispatch->input.mouse_y < origin_y + 467) {
            focused = 13;
        }
        if (focused != dispatch->campaign_menu_focused_button) {
            dispatch->campaign_menu_focused_button = focused;
            dispatch->host->graphics.redraw_requested = 1;
        }
        if (dispatch->input.mouse_right_went_up != 0 &&
            dispatch->right_press_window == 0x17 &&
            (mode == 1 || latest == 0)) {
            open_selected_player_menu(dispatch);
            dispatch->host->graphics.redraw_requested = 1;
        } else if (dispatch->left_pressed_this_frame != 0 &&
            dispatch->left_press_window == 0x17) {
            if (focused >= 1 && focused <= 9) {
                PharaohCampaignMenuState state =
                    campaign_menu_state(dispatch);
                pharaoh_campaign_menu_select_period(&state,
                    (uint32_t)(focused - 1));
            } else if (focused == 10 && mode == 1) {
                PharaohCampaignMenuState state =
                    campaign_menu_state(dispatch);
                const PharaohCampaignMenuOperations operations = {
                    dispatch, main_menu_open_map_selection,
                    campaign_menu_play_period_intro,
                    campaign_menu_start_campaign,
                    campaign_menu_begin_briefing
                };
                (void)pharaoh_campaign_menu_open_individual_missions(
                    &state, &operations);
            } else if (focused == 12 && selected_enabled) {
                PharaohCampaignMenuState state =
                    campaign_menu_state(dispatch);
                const PharaohCampaignMenuOperations operations = {
                    dispatch, main_menu_open_map_selection,
                    campaign_menu_play_period_intro,
                    campaign_menu_start_campaign,
                    campaign_menu_begin_briefing
                };
                (void)pharaoh_campaign_menu_start_selected(
                    &state, &operations);
            } else if (focused == 13 && mode == 1) {
                map_selection_back(dispatch);
            }
        }
        if (dispatch->refresh.current_window_id == 0x17 &&
            dispatch->host->graphics.redraw_requested != 0) {
            pharaoh_windows_startup_host_draw_campaign_menu(dispatch->host,
                dispatch->player_selection.player_name,
                (int32_t)dispatch->menu_field_7027ac,
                (int32_t)dispatch->menu_field_7027b0,
                (int32_t)dispatch->menu_campaign_mode_6fdc98,
                dispatch->campaign_menu_focused_button);
        }
        dispatch->host->current_window_id =
            dispatch->refresh.current_window_id;
        dispatch->left_pressed_this_frame = 0;
        dispatch->input.mouse_left_went_up = 0;
        dispatch->input.mouse_right_went_up = 0;
        dispatch->input.video_input_flag_a = 0;
        dispatch->input.video_input_flag_b = 0;
        pharaoh_graphics_refresh_screen(&dispatch->refresh,
            &dispatch->refresh_operations, 0);
        return;
    }
    if (dispatch->refresh.current_window_id == 0x51) {
        const int32_t origin_x =
            (dispatch->host->graphics.render_width - 640) / 2;
        const int32_t origin_y =
            (dispatch->host->graphics.render_height - 480) / 2;
        const int32_t panel_x = origin_x + 110;
        const int32_t panel_y = origin_y + 30;
        int32_t focused = 0;

        for (int32_t index = 0; index < 42 && focused == 0; ++index) {
            const int32_t column = index >= 21;
            const int32_t row = index - column * 21;
            const int32_t left = panel_x + (column != 0 ? 230 : 5);
            const int32_t top = panel_y + 14 + row * 20;
            if (dispatch->input.mouse_x >= left &&
                dispatch->input.mouse_x < left + 96 &&
                dispatch->input.mouse_y >= top &&
                dispatch->input.mouse_y < top + 18) {
                focused = index + 1;
            }
        }
        if (focused == 0 && dispatch->input.mouse_x >= panel_x + 325 &&
            dispatch->input.mouse_x < panel_x + 400 &&
            dispatch->input.mouse_y >= panel_y + 414 &&
            dispatch->input.mouse_y < panel_y + 432) {
            focused = 43;
        }
        if (dispatch->input.mouse_right_went_up != 0 &&
            dispatch->right_press_window == 0x51) {
            pharaoh_player_name_choices_close(
                &dispatch->front_menu_action_reset,
                &dispatch->refresh.current_window_id,
                &dispatch->host->graphics.redraw_requested);
            dispatch->host->current_window_id =
                dispatch->refresh.current_window_id;
        } else if (dispatch->left_pressed_this_frame != 0 &&
            dispatch->left_press_window == 0x51) {
            if (focused == 43) {
                pharaoh_player_name_choices_close(
                    &dispatch->front_menu_action_reset,
                    &dispatch->refresh.current_window_id,
                    &dispatch->host->graphics.redraw_requested);
                dispatch->host->current_window_id =
                    dispatch->refresh.current_window_id;
            } else if (focused >= 1 && focused <= 42 &&
                focused != 1 && focused != 22) {
                const int32_t choice = focused - 1;
                const char *name;
                pharaoh_text_select(&dispatch->host->text_selection,
                    197, choice);
                name = dispatch->host->text_selection.current;
                if (name != NULL) {
                    (void)strncpy_s(
                        dispatch->player_selection.player_name,
                        sizeof(dispatch->player_selection.player_name),
                        name, _TRUNCATE);
                    dispatch->player_selection.selected_name_choice = choice;
                    dispatch->host->graphics.redraw_requested = 1;
                }
            }
        }
        if (dispatch->refresh.current_window_id == 0x51) {
            pharaoh_windows_startup_host_draw_player_name_choices(
                dispatch->host, &dispatch->player_selection, focused);
            pharaoh_graphics_refresh_screen(&dispatch->refresh,
                &dispatch->refresh_operations, 0);
        }
        dispatch->player_selection_focused_button = focused;
        dispatch->left_pressed_this_frame = 0;
        dispatch->input.mouse_left_went_up = 0;
        dispatch->input.mouse_right_went_up = 0;
        dispatch->input.video_input_flag_a = 0;
        dispatch->input.video_input_flag_b = 0;
        return;
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
        if (dispatch->input.mouse_right_went_up != 0 &&
            dispatch->right_press_window == 0x2f) {
            dispatch->refresh.current_window_id = 0;
            dispatch->host->current_window_id = 0;
            dispatch->host->graphics.redraw_requested = 1;
            dispatch->menu_focused_button = 0;
        }
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
                load_clicked_player_profile(dispatch, focused - 1);
            } else if (focused == 13) {
                begin_new_family(dispatch);
            } else if (focused == 14) {
                show_family_message(dispatch, 0x5a, 2,
                    FAMILY_MESSAGE_DELETE);
            } else if (focused == 15) {
                if (dispatch->player_selection.player_name[0] == '\0') {
                    show_family_message(dispatch, 0x5e, 1,
                        FAMILY_MESSAGE_NONE);
                } else {
                    open_selected_player_menu(dispatch);
                    dispatch->host->graphics.redraw_requested = 1;
                    dispatch->player_selection_focused_button = 0;
                }
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
        const int32_t list_top = origin_y +
            (dispatch->custom_map_selection.visible_rows == 13 ? 252 : 220);
        int32_t focused_button = 0;
        int32_t hovered_row = -1;
        const int32_t up_y = origin_y +
            (dispatch->custom_map_selection.visible_rows == 13 ? 247 : 215);
        const int32_t difficulty_visible =
            pharaoh_map_selection_difficulty_buttons_visible(
                dispatch->menu_selection_flag_605a3c,
                dispatch->menu_map_changed_704db0,
                dispatch->menu_map_mode_704dac);
        if (dispatch->input.mouse_x >= origin_x + 24 &&
            dispatch->input.mouse_x < origin_x + 280 &&
            dispatch->input.mouse_y >= list_top &&
            dispatch->input.mouse_y < list_top +
                dispatch->custom_map_selection.visible_rows * 16) {
            hovered_row = (dispatch->input.mouse_y - list_top) / 16;
            if (dispatch->custom_map_selection.first_visible_file +
                    hovered_row >= dispatch->custom_map_files.count) {
                hovered_row = -1;
            }
        }
        if (dispatch->input.mouse_x >= origin_x + 282 &&
            dispatch->input.mouse_x < origin_x + 316 &&
            dispatch->input.mouse_y >= up_y &&
            dispatch->input.mouse_y < up_y + 34) focused_button = 1;
        else if (dispatch->input.mouse_x >= origin_x + 282 &&
            dispatch->input.mouse_x < origin_x + 316 &&
            dispatch->input.mouse_y >= origin_y + 435 &&
            dispatch->input.mouse_y < origin_y + 469) focused_button = 2;
        else if (dispatch->input.mouse_x >= origin_x + 335 &&
            dispatch->input.mouse_x < origin_x + 362 &&
            dispatch->input.mouse_y >= origin_y + 440 &&
            dispatch->input.mouse_y < origin_y + 467) focused_button = 3;
        else if (pharaoh_map_selection_image_button_count(
                &dispatch->custom_map_selection) == 4 &&
            dispatch->input.mouse_x >= origin_x + 600 &&
            dispatch->input.mouse_x < origin_x + 627 &&
            dispatch->input.mouse_y >= origin_y + 440 &&
            dispatch->input.mouse_y < origin_y + 467) focused_button = 4;
        else if (difficulty_visible != 0 &&
            dispatch->input.mouse_y >= origin_y + 392 &&
            dispatch->input.mouse_y < origin_y + 409 &&
            dispatch->input.mouse_x >= origin_x + 368 &&
            dispatch->input.mouse_x < origin_x + 385) focused_button = 5;
        else if (difficulty_visible != 0 &&
            dispatch->input.mouse_y >= origin_y + 392 &&
            dispatch->input.mouse_y < origin_y + 409 &&
            dispatch->input.mouse_x >= origin_x + 385 &&
            dispatch->input.mouse_x < origin_x + 402) focused_button = 6;
        else if (dispatch->menu_map_changed_704db0 == 1 &&
            dispatch->input.mouse_x >= origin_x + 350 &&
            dispatch->input.mouse_x < origin_x + 600 &&
            dispatch->input.mouse_y >= origin_y + 410 &&
            dispatch->input.mouse_y < origin_y + 433) focused_button = 9;
        else if (dispatch->menu_map_changed_704db0 == 1 &&
            dispatch->input.mouse_x >= origin_x + 32 &&
            dispatch->input.mouse_x < origin_x + 192 &&
            dispatch->input.mouse_y >= origin_y + 208 &&
            dispatch->input.mouse_y < origin_y + 233) focused_button = 7;
        else if (dispatch->menu_map_changed_704db0 == 1 &&
            dispatch->input.mouse_x >= origin_x + 195 &&
            dispatch->input.mouse_x < origin_x + 288 &&
            dispatch->input.mouse_y >= origin_y + 208 &&
            dispatch->input.mouse_y < origin_y + 233) focused_button = 8;
        if (focused_button != dispatch->map_selection_focused_button ||
            hovered_row != dispatch->map_selection_hovered_row) {
            dispatch->map_selection_focused_button = focused_button;
            dispatch->map_selection_hovered_row = hovered_row;
            dispatch->host->graphics.redraw_requested = 1;
        }
        if (dispatch->input.mouse_right_went_up != 0 &&
            dispatch->right_press_window == 0x3d) {
            map_selection_back(dispatch);
        }
        if (dispatch->left_pressed_this_frame != 0 &&
            dispatch->left_press_window == 0x3d &&
            dispatch->input.mouse_x >= origin_x + 24 &&
            dispatch->input.mouse_x < origin_x + 280 &&
            dispatch->input.mouse_y >= list_top &&
            dispatch->input.mouse_y < list_top +
                dispatch->custom_map_selection.visible_rows * 16 &&
            map_selection_activate_row(dispatch,
                (dispatch->input.mouse_y - list_top) / 16) != 0) {
            dispatch->host->graphics.redraw_requested = 1;
        }
        if (dispatch->left_pressed_this_frame != 0 &&
            dispatch->left_press_window == 0x3d &&
            dispatch->input.mouse_x >= origin_x + 282 &&
            dispatch->input.mouse_x < origin_x + 316) {
            if (dispatch->input.mouse_y >= up_y &&
                dispatch->input.mouse_y < up_y + 34) {
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
            (focused_button == 5 || focused_button == 6)) {
            map_selection_adjust_difficulty_ui(dispatch,
                focused_button == 5);
        }
        if (dispatch->left_pressed_this_frame != 0 &&
            dispatch->left_press_window == 0x3d &&
            dispatch->menu_map_changed_704db0 == 1 &&
            dispatch->input.mouse_x >= origin_x + 350 &&
            dispatch->input.mouse_x < origin_x + 600 &&
            dispatch->input.mouse_y >= origin_y + 410 &&
            dispatch->input.mouse_y < origin_y + 433) {
            pharaoh_map_selection_toggle_mode(
                &dispatch->menu_map_mode_704dac,
                &dispatch->host->graphics.redraw_requested);
        }
        if (dispatch->left_pressed_this_frame != 0 &&
            dispatch->left_press_window == 0x3d &&
            focused_button == 8) {
            PharaohCampaignMenuState state = campaign_menu_state(dispatch);
            (void)pharaoh_campaign_menu_open_campaigns(&state);
            dispatch->host->current_window_id = 0x17;
            dispatch->campaign_menu_focused_button = 0;
        }
        if (dispatch->left_pressed_this_frame != 0 &&
            dispatch->left_press_window == 0x3d &&
            dispatch->input.mouse_x >= origin_x + 335 &&
            dispatch->input.mouse_x < origin_x + 362 &&
            dispatch->input.mouse_y >= origin_y + 440 &&
            dispatch->input.mouse_y < origin_y + 467) {
            map_selection_back(dispatch);
        }
        if (dispatch->refresh.current_window_id == 0x3d &&
            dispatch->host->graphics.redraw_requested != 0) {
            pharaoh_windows_startup_host_draw_custom_map_list(dispatch->host,
                &dispatch->custom_map_selection,
                dispatch->menu_map_changed_704db0 == 1,
                dispatch->menu_selection_flag_605a3c,
                dispatch->menu_map_mode_704dac,
                dispatch->map_selection_focused_button,
                dispatch->map_selection_hovered_row);
        }
        dispatch->host->current_window_id = dispatch->refresh.current_window_id;
        dispatch->left_pressed_this_frame = 0;
        dispatch->input.mouse_left_went_up = 0;
        dispatch->input.mouse_right_went_up = 0;
        dispatch->input.video_input_flag_a = 0;
        dispatch->input.video_input_flag_b = 0;
        /* This frame belongs to window 0x3d even when its callback changes
           the active window.  Native input dispatch returns after that
           callback; falling through would dispatch the new window a second
           time with transition-owned input state that is not bound here. */
        pharaoh_graphics_refresh_screen(&dispatch->refresh,
            &dispatch->refresh_operations, 0);
        return;
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
