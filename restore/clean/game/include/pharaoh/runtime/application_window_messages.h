#ifndef PHARAOH_RUNTIME_APPLICATION_WINDOW_MESSAGES_H
#define PHARAOH_RUNTIME_APPLICATION_WINDOW_MESSAGES_H

#include <stdint.h>

#include "pharaoh/runtime/graphics_refresh.h"
#include "pharaoh/runtime/graphics_startup.h"
#include "pharaoh/runtime/smack_video_shutdown.h"

enum {
    PHARAOH_WINDOW_MESSAGE_CLOSE = 0x0010,
    PHARAOH_NATIVE_WINDOW_CLOSE_REQUESTED = 3
};

/* Handles the native WM_CLOSE state write. The window procedure still runs
   its common DefWindowProcA path after this state update. */
int32_t pharaoh_application_window_record_close_message(
    uint32_t message, uint8_t *native_close_state);

typedef struct PharaohApplicationWindowPaintContext {
    PharaohGraphicsRefreshState *graphics_state;
    const PharaohGraphicsRefreshOperations *graphics_operations;
    uint8_t *redraw_requested;
    uint8_t *city_redraw_requested;
} PharaohApplicationWindowPaintContext;

typedef struct PharaohApplicationWindowDestroyContext {
    void **native_window_slot;
    PharaohGraphicsStartupState *graphics_state;
    const PharaohGraphicsStartupOperations *graphics_operations;
    const PharaohSmackVideoShutdownContext *video_shutdown;
} PharaohApplicationWindowDestroyContext;

typedef struct PharaohApplicationWindowMoveSizeContext {
    void *window;
    int32_t fullscreen_display_mode;
    int32_t *paused_state;
    int32_t *activation_refresh_pending;
    uint8_t *redraw_requested;
    uint8_t *city_redraw_requested;
    PharaohGraphicsWindowRect *screen_bounds;
    PharaohGraphicsRefreshState *graphics_state;
    const PharaohGraphicsRefreshOperations *graphics_operations;
    void *report_user_data;
    void (*report_paused)(void *user_data);
} PharaohApplicationWindowMoveSizeContext;

/* Handles native WM_PAINT effects, then leaves return-value handling to the
   window procedure's shared DefWindowProcA path. */
int32_t pharaoh_application_window_handle_paint_message(
    uint32_t message, void *window,
    const PharaohApplicationWindowPaintContext *context);

/* Handles native WM_DESTROY effects, then leaves return-value handling to the
   window procedure's shared DefWindowProcA path. */
int32_t pharaoh_application_window_handle_destroy_message(
    uint32_t message,
    const PharaohApplicationWindowDestroyContext *context);

/* Handles the shared native WM_MOVE and WM_SIZE branch. */
int32_t pharaoh_application_window_handle_move_size_message(
    uint32_t message,
    const PharaohApplicationWindowMoveSizeContext *context);


/* Original entries 0x004CFFC0 and 0x004CFF90. */
void pharaoh_application_window_clear_pause_if_foreground(
    void *window, int32_t *paused_state);
void pharaoh_application_window_pause_if_minimized(
    void *window, int32_t *paused_state, void *report_user_data,
    void (*report_paused)(void *user_data));

#endif
