#include "pharaoh/runtime/smack_video_shutdown.h"

void pharaoh_smack_video_shutdown(
    const PharaohSmackVideoShutdownContext *context)
{
    if (context->video_system_active == 0) {
        return;
    }

    if (context->sound_device_already_closed == 0) {
        context->operations->close_sound_device(
            context->operations->user_data, context->sound_state);
    }

    if (*context->movie_handle != 0) {
        context->operations->close_movie(
            context->operations->user_data, *context->movie_handle);
        *context->movie_handle = 0;
    }

    context->operations->finish_sound_shutdown(
        context->operations->user_data);
}
