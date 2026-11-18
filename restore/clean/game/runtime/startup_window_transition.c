#include "pharaoh/runtime/startup_window_transition.h"

enum {
    STARTUP_WINDOW_FRONT_MENU = 0x00,
    STARTUP_WINDOW_TIMED_TRANSITION = 0x04,
    STARTUP_WINDOW_BINK_PLAYING = 0x4a,
    STARTUP_WINDOW_LOGO = 0x3c,
    STARTUP_WINDOW_CREDITS_VIDEO = 0x41,
    STARTUP_WINDOW_INTRO_VIDEO = 0x40,
    STARTUP_WINDOW_LOGO_VIDEO = 0x44,
    STARTUP_WINDOW_SMACKER_PLAYING = 0x13,
    STARTUP_VIDEO_SYSTEM_FULLSCREEN = 2,
    STARTUP_VIDEO_WIDTH = 500,
    STARTUP_VIDEO_HEIGHT = 0x124,
    STARTUP_CREDITS_WIDTH = 0x280,
    STARTUP_CREDITS_HEIGHT = 0x1e0,
    STARTUP_VIDEO_FLAGS = 1
};

static void initialize_dispatcher_entry(
    const PharaohStartupWindowTransitionContext *context)
{
    const PharaohStartupWindowInputState *const input = context->input;
    const PharaohStartupWindowDispatcherEntryState *const entry =
        context->entry_state;

    *entry->entry_result_slot_0 = 0;
    *entry->entry_result_slot_1 = 0;
    *entry->entry_result_slot_2 = 0;
    *entry->entry_result_slot_3 = 0;
    *input->mouse_x_ui = input->mouse_x;
    *input->mouse_y_ui = input->mouse_y;

    if (*entry->input_mode == 2) {
        *entry->input_mode = 0;
        *entry->redraw_requested = 1;
    }
}

static void play_centered_startup_video(
    const PharaohStartupWindowTransitionContext *context,
    const char *filename, int32_t video_width, int32_t video_height,
    int32_t return_window_id)
{
    PharaohSmackerWindowVideoState *const video = context->video;
    video->video_system_state = STARTUP_VIDEO_SYSTEM_FULLSCREEN;
    context->clear_video_buffer(context->display_context);
    (void)pharaoh_smacker_window_video_start(video, filename,
        (context->screen_width - video_width) / 2,
        (context->screen_height - video_height) / 2,
        STARTUP_VIDEO_FLAGS, return_window_id, context->video_operations);
}

void pharaoh_startup_window_transition_dispatch(
    const PharaohStartupWindowTransitionContext *context)
{
    PharaohSmackerWindowVideoState *const video = context->video;
    initialize_dispatcher_entry(context);

    switch (video->graphics_refresh_state->current_window_id) {
    case STARTUP_WINDOW_FRONT_MENU:
        pharaoh_front_menu_handle_input(context->front_menu_input);
        return;
    case STARTUP_WINDOW_TIMED_TRANSITION:
        pharaoh_timed_window_transition_dispatch(context->timed_transition);
        return;
    case STARTUP_WINDOW_LOGO_VIDEO:
        play_centered_startup_video(context, "smk\\logo.smk",
            STARTUP_VIDEO_WIDTH, STARTUP_VIDEO_HEIGHT,
            STARTUP_WINDOW_INTRO_VIDEO);
        return;
    case STARTUP_WINDOW_INTRO_VIDEO:
        play_centered_startup_video(context, "smk\\intro.smk",
            STARTUP_VIDEO_WIDTH, STARTUP_VIDEO_HEIGHT,
            STARTUP_WINDOW_CREDITS_VIDEO);
        return;
    case STARTUP_WINDOW_CREDITS_VIDEO:
        play_centered_startup_video(context, "smk\\credits.smk",
            STARTUP_CREDITS_WIDTH, STARTUP_CREDITS_HEIGHT,
            STARTUP_WINDOW_LOGO);
        return;
    case STARTUP_WINDOW_LOGO:
        if (context->input->mouse_right_went_up == 0 &&
            context->input->mouse_left_went_up == 0) {
            return;
        }
        video->graphics_refresh_state->current_window_id =
            STARTUP_WINDOW_FRONT_MENU;
        *video->redraw_requested = 1;
        return;
    case STARTUP_WINDOW_SMACKER_PLAYING:
        if (*context->video_dialog_overlay_active != 0) {
            context->dispatch_video_dialog_input(
                context->video_dialog_input_user_data);
            return;
        }
        if (context->input->video_input_flag_a != 0 ||
            context->input->video_input_flag_b != 0) {
            pharaoh_smacker_window_video_stop(video,
                context->video_stop_operations);
        }
        return;
    case STARTUP_WINDOW_BINK_PLAYING:
        if (*context->video_dialog_overlay_active != 0) {
            context->dispatch_video_dialog_input(
                context->video_dialog_input_user_data);
            return;
        }
        if (context->bink_player == NULL) {
            return;
        }
        if (context->input->mouse_right_went_up != 0 ||
            context->input->mouse_left_went_up != 0) {
            (void)pharaoh_video_player_end(context->bink_player,
                context->bink_runtime, context->bink_backend);
        }
        return;
    default:
        return;
    }
}
