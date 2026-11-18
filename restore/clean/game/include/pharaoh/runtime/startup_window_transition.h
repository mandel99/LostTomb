#ifndef PHARAOH_RUNTIME_STARTUP_WINDOW_TRANSITION_H
#define PHARAOH_RUNTIME_STARTUP_WINDOW_TRANSITION_H

#include "pharaoh/runtime/smacker_window_video_start.h"
#include "pharaoh/runtime/timed_window_transition.h"
#include "pharaoh/runtime/video_player.h"
#include "pharaoh/ui/front_menu_input.h"

#include <stdint.h>

typedef struct PharaohStartupWindowInputState {
    int32_t mouse_x;
    int32_t mouse_y;
    int32_t *mouse_x_ui;
    int32_t *mouse_y_ui;
    uint8_t mouse_right_went_up;
    uint8_t mouse_left_went_up;
    /* Native video-window inputs are observed as two byte tests; their wider
       input meanings are not established by the current evidence. */
    uint8_t video_input_flag_a;
    uint8_t video_input_flag_b;
} PharaohStartupWindowInputState;

typedef struct PharaohStartupWindowDispatcherEntryState {
    /* The native entry clears four independent DWORDs; their individual
       gameplay meanings have not been established. */
    int32_t *entry_result_slot_0;
    int32_t *entry_result_slot_1;
    int32_t *entry_result_slot_2;
    int32_t *entry_result_slot_3;
    int32_t *input_mode;
    uint8_t *redraw_requested;
} PharaohStartupWindowDispatcherEntryState;

typedef struct PharaohStartupWindowTransitionContext {
    PharaohSmackerWindowVideoState *video;
    const PharaohStartupWindowInputState *input;
    const PharaohStartupWindowDispatcherEntryState *entry_state;
    PharaohFrontMenuInputContext *front_menu_input;
    const PharaohTimedWindowTransitionContext *timed_transition;
    PharaohVideoPlayer *bink_player;
    PharaohVideoRuntimeState *bink_runtime;
    const PharaohVideoPlayerBackend *bink_backend;
    int32_t screen_width;
    int32_t screen_height;
    void *display_context;
    void (*clear_video_buffer)(void *display_context);
    const PharaohSmackerMovieOpenOperations *video_operations;
    const PharaohSmackerWindowVideoStopOperations *video_stop_operations;
    const int32_t *video_dialog_overlay_active;
    void *video_dialog_input_user_data;
    void (*dispatch_video_dialog_input)(void *user_data);
} PharaohStartupWindowTransitionContext;

/* Implements the shared entry prologue, window-zero front-menu input branch,
   and startup-video/logo branches of native 0x004D8310. Other window IDs
   remain with its full owner. */
void pharaoh_startup_window_transition_dispatch(
    const PharaohStartupWindowTransitionContext *context);

#endif
