#include "video_player.h"
RestoreVideoPlayer *FUN_004133d0(RestoreVideoPlayer *player, HWND window)
{
    player->window=window;
    player->stopped=1;
    player->movie=0;
    player->buffer=0;
    return player;
}
