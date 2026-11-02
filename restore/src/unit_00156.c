#include "video_player.h"
unsigned char FUN_00413420(RestoreVideoPlayer *player)
{
    *(unsigned char *)0x00e38ddau=(unsigned char)player->saved_mode;
    player->stopped=1;
    *(unsigned char *)0x00e38dcau=1;
    FUN_004133f0(player);
    return 1;
}
