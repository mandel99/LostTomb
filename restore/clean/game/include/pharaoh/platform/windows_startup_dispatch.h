#ifndef PHARAOH_PLATFORM_WINDOWS_STARTUP_DISPATCH_H
#define PHARAOH_PLATFORM_WINDOWS_STARTUP_DISPATCH_H

#include "pharaoh/platform/windows_startup_host.h"
#include "pharaoh/platform/windows_smacker.h"
#include "pharaoh/runtime/startup_window_transition.h"
#include "pharaoh/ui/high_scores.h"

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
    PharaohHighScoresState high_scores;
    int32_t entry_results[4];
    int32_t mouse_x_ui, mouse_y_ui, input_mode, dialog_active;
    uint8_t left_press_active, right_press_active;
    uint8_t left_press_window, right_press_window;
    uint8_t left_pressed_this_frame;
    int32_t menu_focused_button;
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

#endif
