#ifndef PHARAOH_PLATFORM_WINDOWS_STARTUP_DISPATCH_H
#define PHARAOH_PLATFORM_WINDOWS_STARTUP_DISPATCH_H

#include "pharaoh/platform/windows_startup_host.h"
#include "pharaoh/platform/windows_smacker.h"
#include "pharaoh/runtime/startup_window_transition.h"
#include "pharaoh/ui/high_scores.h"
#include "pharaoh/ui/main_menu_open.h"
#include "pharaoh/ui/message_box.h"
#include "pharaoh/runtime/file_list.h"

typedef struct PharaohWindowsStartupDispatch {
    PharaohWindowsStartupHost *host;
    PharaohWindowsSmacker smacker;
    PharaohSmackerMovieOpenOperations movie_open;
    PharaohSmackerVideoDrawOperations movie_draw;
    PharaohSmackerWindowVideoStopOperations movie_stop;
    PharaohSmackerWindowVideoState video;
    PharaohGraphicsRefreshState refresh;
    PharaohGraphicsRefreshOperations refresh_operations;
    PharaohStartupWindowInputState input;
    PharaohStartupWindowDispatcherEntryState entry;
    PharaohStartupWindowTransitionContext transition;
    PharaohTimedWindowTransitionContext timed_transition;
    PharaohHighScoresState high_scores;
    PharaohHighScoresOperations high_scores_operations;
    PharaohMainMenuWindowState main_menu_window;
    PharaohMessageBoxState exit_message_box;
    PharaohFileList player_profile_files;
    PharaohPlayerSelectionState player_selection;
    PharaohFileList saved_game_files;
    int32_t saved_game_first_visible, saved_game_selected_row;
    PharaohFileList custom_map_files;
    PharaohCustomMapSelectionState custom_map_selection;
    int32_t entry_results[4];
    int32_t mouse_x_ui, mouse_y_ui, input_mode, dialog_active;
    uint8_t left_press_active, right_press_active;
    uint8_t left_press_window, right_press_window;
    uint8_t left_pressed_this_frame, right_pressed_this_frame;
    uint8_t mouse_button_state;
    uint8_t exit_message_box_active, exit_post_close_requested;
    uint8_t front_menu_action_active, save_selection_mode;
    uint32_t front_menu_action_reset, continue_campaign_pending;
    uint32_t menu_selection_flag_605a3c, menu_field_7027ac;
    uint32_t menu_field_7027b0, menu_campaign_mode_6fdc98;
    uint32_t menu_campaign_mode_e38ebc, menu_map_mode_704dac;
    uint32_t menu_map_changed_704db0, menu_load_return_window;
    uint8_t menu_selection_flag_6de515;
    uint32_t message_box_display_unavailable, message_box_display_ready;
    uint32_t campaign_flow_mode;
    uint32_t timed_transition_value;
    uint8_t timed_transition_next_window;
    int32_t menu_focused_button;
    int32_t main_menu_focused_button;
    int32_t campaign_menu_focused_button;
    int32_t map_selection_focused_button;
    int32_t map_selection_hovered_row;
    int32_t player_selection_focused_button;
    int32_t family_message_text_entry, family_message_previous_profile;
    uint8_t family_message_previous_window, family_message_button_count;
    uint8_t family_message_action, family_message_focused_button;
} PharaohWindowsStartupDispatch;

/* Binds the boot/video subset. The caller owns the menu contexts used after
   reaching state zero; this binding does not supply replacement menu logic. */
int32_t pharaoh_windows_startup_dispatch_bind(
    PharaohWindowsStartupDispatch *dispatch, PharaohWindowsStartupHost *host);
void pharaoh_windows_startup_dispatch_close(
    PharaohWindowsStartupDispatch *dispatch);
void pharaoh_windows_startup_dispatch_frame(void *user_data);
void pharaoh_windows_startup_dispatch_mouse(
    PharaohWindowsStartupDispatch *dispatch, UINT message, LPARAM lparam);
void pharaoh_windows_startup_dispatch_character(
    PharaohWindowsStartupDispatch *dispatch, WPARAM character);
void pharaoh_windows_startup_dispatch_escape(
    PharaohWindowsStartupDispatch *dispatch);

#endif
