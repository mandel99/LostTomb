#include "pharaoh/runtime/video_player.h"

#include <stddef.h>

_Static_assert(offsetof(PharaohVideoPlayer, stopped) == 4,
    "native video player stopped flag offset");
_Static_assert(offsetof(PharaohVideoPlayer, window) == 8,
    "native video player window offset");
_Static_assert(sizeof(void *) != 4 ||
    (offsetof(PharaohVideoPlayer, buffer) == 12 &&
     offsetof(PharaohVideoPlayer, movie) == 16 &&
     sizeof(PharaohVideoPlayer) == 28),
    "video player retains the native 32-bit object layout");

void pharaoh_video_player_initialize(PharaohVideoPlayer *player,
                                     void *window)
{
    player->window = window;
    player->stopped = UINT8_C(1);
    player->movie = NULL;
    player->buffer = NULL;
}

void pharaoh_video_player_close(PharaohVideoPlayer *player,
                                const PharaohVideoPlayerBackend *backend)
{
    if (player->movie != NULL) {
        backend->close_movie(backend->user_data, player->movie);
        player->movie = NULL;
    }

    if (player->buffer != NULL) {
        backend->close_buffer(backend->user_data, player->buffer);
        player->buffer = NULL;
    }
}

uint8_t pharaoh_video_player_end(PharaohVideoPlayer *player,
                                 PharaohVideoRuntimeState *state,
                                 const PharaohVideoPlayerBackend *backend)
{
    *state->display_mode = (uint8_t)player->saved_mode;
    player->stopped = UINT8_C(1);
    *state->redraw_requested = UINT8_C(1);

    if (player->movie != NULL) {
        backend->close_movie(backend->user_data, player->movie);
        player->movie = NULL;
    }

    if (player->buffer != NULL) {
        backend->close_buffer(backend->user_data, player->buffer);
        player->buffer = NULL;
    }

    return UINT8_C(1);
}

