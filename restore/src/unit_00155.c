#include "video_player.h"
#include "media_bindings.h"
void FUN_004133f0(RestoreVideoPlayer *player)
{
    if(player->movie) {
        _BinkClose_4(player->movie);
        player->movie=0;
    }
    if(player->buffer) {
        _BinkBufferClose_4(player->buffer);
        player->buffer=0;
    }
}
