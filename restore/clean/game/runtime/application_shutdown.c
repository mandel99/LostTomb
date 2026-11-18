#include "pharaoh/runtime/application_shutdown.h"

void pharaoh_application_shutdown_finish(
    const PharaohApplicationShutdownContext *context)
{
    PharaohVideoPlayer *const player = *context->video_player;

    if (player != NULL) {
        pharaoh_video_player_close(player, context->video_backend);
        context->operations->release_video_player_storage(
            context->operations->user_data, player);
        *context->video_player = NULL;
    }

    context->operations->co_uninitialize(context->operations->user_data);
    context->operations->report_normal_exit(context->operations->user_data);
}
