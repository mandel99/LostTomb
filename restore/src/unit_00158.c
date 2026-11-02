#include "video_player.h"
#include "media_bindings.h"
#include "sound_streams.h"
#include "startup_trace.h"
#include <stdio.h>
#include <string.h>

/* Native 41359d..4135bc always uses this specific sound controller. */
unsigned char FUN_00413580(RestoreVideoPlayer *player,const char *filename,int x,int y)
{
    char path[100];
    FILE *probe;
    int length;
    uintptr_t audio=*(uintptr_t *)0x012663acu;
    if(player->movie) _BinkClose_4(player->movie);
    if(audio) {
        restore_sound_close_stream((void *)0x00e92bf8u,0);
        restore_sound_close_stream((void *)0x00e92bf8u,2);
        restore_sound_close_stream((void *)0x00e92bf8u,1);
        _BinkSetSoundSystem_8((uintptr_t)_BinkOpenMiles_4,*(uintptr_t *)(audio+0x178));
    }
    length=snprintf(path,sizeof(path),"%s",filename);
    if(length<0 || (size_t)length>=sizeof(path)) {
        player->movie=0;
        return 0;
    }
    probe=fopen(path,"rb");
    if(probe) fclose(probe);
    else {
        length=snprintf(path,sizeof(path),"%c:\\%s",*(unsigned char *)0x00e38e62u,filename);
        if(length<0 || (size_t)length>=sizeof(path)) {
            player->movie=0;
            return 0;
        }
    }
    player->movie=(uintptr_t)_BinkOpen_8((uintptr_t)path,0);
    restore_startup_trace("STARTUP BinkOpen path=%s movie=%08lx\n",path,(unsigned long)player->movie);
    if(!player->movie) {
        OutputDebugStringA((const char *)0x0057639cu);
        return 0;
    }
    player->saved_mode=*(unsigned char *)0x00e38ddau;
    *(unsigned char *)0x00e38ddau=0x4a;
    player->stopped=0;
    *(unsigned char *)0x00e38dcau=0;
    player->x=x;
    player->y=y;
    FUN_00413690(player);
    return 1;
}
