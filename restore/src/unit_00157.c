#include "video_player.h"
#include "media_bindings.h"
#include <ddraw.h>
#include <string.h>
extern int FUN_00427510(int error);
extern void FUN_004ce6e0(char *message,int detail,int number);

unsigned char FUN_00413470(RestoreVideoPlayer *player)
{
    LPDIRECTDRAWSURFACE surface;
    DDSURFACEDESC *description=(DDSURFACEDESC *)0x00605b78u;
    HRESULT result;
    uintptr_t destination;
    int surface_type;
    if(!player->movie || player->stopped) return 0;
    surface=*(LPDIRECTDRAWSURFACE *)0x00e39178u;
    memset(description,0,sizeof(*description));
    description->dwSize=sizeof(*description);
    result=surface->lpVtbl->Lock(surface,0,description,DDLOCK_WAIT,0);
    while(result==DDERR_SURFACELOST) {
        if(surface->lpVtbl->Restore(surface)!=DD_OK) return 1;
        result=surface->lpVtbl->Lock(surface,0,description,DDLOCK_WAIT,0);
    }
    if(result!=DD_OK) {
        FUN_004ce6e0((char *)0x00576364u,0,FUN_00427510((int)result)+1);
        *(uint32_t *)0x00e92e04u=0;
        return 1;
    }
    /* Match 32-bit add/imul/lea wrapping, including signed surface pitch. */
    destination=(uintptr_t)description->lpSurface+
        (uint32_t)description->lPitch*((uint32_t)player->y+*(uint32_t *)0x00e39134u)+
        2u*((uint32_t)player->x+*(uint32_t *)0x00e39130u);
    surface_type=_BinkDDSurfaceType_4((uintptr_t)surface);
    _BinkCopyToBuffer_28(player->movie,destination,(uintptr_t)description->lPitch,
        *(uint32_t *)(player->movie+4),0,0,(uintptr_t)surface_type);
    surface->lpVtbl->Unlock(surface,description->lpSurface);
    return 1;
}
