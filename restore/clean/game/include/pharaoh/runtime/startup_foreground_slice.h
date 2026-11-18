#ifndef PHARAOH_RUNTIME_STARTUP_FOREGROUND_SLICE_H
#define PHARAOH_RUNTIME_STARTUP_FOREGROUND_SLICE_H

#include "pharaoh/ui/front_menu_render.h"
#include "pharaoh/runtime/fast_forward_note.h"
#include "pharaoh/runtime/smacker_window_video_start.h"

#include <stdint.h>

typedef struct PharaohStartupForegroundSliceContext {
    uint8_t current_window_id;
    uint8_t *window_ui_redraw;
    const uint8_t *native_foreground_note_gate;
    const PharaohFrontMenuRenderContext *front_menu;
    const PharaohFastForwardNoteContext *foreground_note;
    PharaohSmackerWindowVideoState *smacker_video;
    const PharaohSmackerVideoDrawOperations *smacker_draw_operations;
    const int32_t *video_dialog_overlay_active;
    void *video_dialog_overlay_user_data;
    void (*draw_video_dialog_overlay)(void *user_data);
} PharaohStartupForegroundSliceContext;

/* Startup/video slice of native 0x004D75D0: windows 0, 0x13, and 0x3C. Other
   windows remain owned by the full, not-yet-translated dispatcher. */
void pharaoh_startup_foreground_slice(
    const PharaohStartupForegroundSliceContext *context);

#endif
