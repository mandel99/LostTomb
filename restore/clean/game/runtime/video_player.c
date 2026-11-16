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
