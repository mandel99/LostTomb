#ifndef PHARAOH_RUNTIME_GRAPHICS_FRAME_RESTORE_H
#define PHARAOH_RUNTIME_GRAPHICS_FRAME_RESTORE_H

#include <stdint.h>
#include "pharaoh/runtime/status_log_format.h"

typedef struct PharaohGraphicsFrameRestoreOperations {
    void *user_data;
    int32_t (*restore_surface)(void *user_data, void *surface);
} PharaohGraphicsFrameRestoreOperations;

typedef struct PharaohGraphicsFrameRestoreContext {
    void *front_surface;
    void *back_surface;
    PharaohStatusLogFormatState *status_state;
    const PharaohStatusLogFormatOperations *status_operations;
    const PharaohGraphicsFrameRestoreOperations *operations;
} PharaohGraphicsFrameRestoreContext;

/* Native entry 0x004CFFE0. */
int32_t pharaoh_graphics_frame_restore_surfaces(
    const PharaohGraphicsFrameRestoreContext *context);

#endif
