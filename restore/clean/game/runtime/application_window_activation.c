#include "pharaoh/runtime/application_window_activation.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "pharaoh/runtime/video_player.h"

int32_t pharaoh_application_window_handle_deactivate_audio(
    uint32_t message, uintptr_t wparam,
    const PharaohApplicationWindowActivateAudioContext *context)
{
    if (message != WM_ACTIVATE || (wparam >> 16) == 0) {
        return 0;
    }

    pharaoh_digital_sound_stream_close(context->sound_manager,
        PHARAOH_VIDEO_STREAM_MUSIC, context->sound_operations);
    pharaoh_digital_sound_stream_close(context->sound_manager,
        PHARAOH_VIDEO_STREAM_CITY, context->sound_operations);
    pharaoh_digital_sound_stream_close(context->sound_manager,
        PHARAOH_VIDEO_STREAM_SPEECH, context->sound_operations);
    *context->foreground_restore_pending = 1;
    return 1;
}
