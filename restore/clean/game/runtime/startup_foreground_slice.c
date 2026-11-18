#include "pharaoh/runtime/startup_foreground_slice.h"

enum {
    STARTUP_FOREGROUND_FRONT_MENU = 0x00,
    STARTUP_FOREGROUND_SMACKER_VIDEO = 0x13,
    STARTUP_FOREGROUND_LOGO = 0x3c
};

void pharaoh_startup_foreground_slice(
    const PharaohStartupForegroundSliceContext *context)
{
    if (context->current_window_id == STARTUP_FOREGROUND_FRONT_MENU) {
        pharaoh_front_menu_render_window_zero(context->front_menu);
    } else if (context->current_window_id ==
            STARTUP_FOREGROUND_SMACKER_VIDEO) {
        (void)pharaoh_smacker_video_draw(context->smacker_video,
            context->smacker_draw_operations);
        if (*context->video_dialog_overlay_active != 0) {
            context->draw_video_dialog_overlay(
                context->video_dialog_overlay_user_data);
        }
    } else if (context->current_window_id != STARTUP_FOREGROUND_LOGO) {
        return;
    }

    *context->window_ui_redraw = 0;
    if (*context->native_foreground_note_gate != 0) {
        pharaoh_fast_forward_note_draw(context->foreground_note);
    }
}
