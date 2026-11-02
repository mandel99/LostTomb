#include "graphics_session.h"
#include <stdio.h>
#include <string.h>
#define s_OK__DD_Released__005d1d00 (*(char (*)[17])0x005d1d00u)

extern void FUN_004ce6e0(char *,int,int);
int FUN_00426ee0(int release_device)
{
    LPDIRECTDRAWCLIPPER clipper=GFX_P(LPDIRECTDRAWCLIPPER,0xe39170);
    LPDIRECTDRAWSURFACE surface;
    LPDIRECTDRAW draw;
    if(clipper) {clipper->lpVtbl->Release(clipper); GFX_P(LPDIRECTDRAWCLIPPER,0xe39170)=0;}
    surface=GFX_P(LPDIRECTDRAWSURFACE,0xe39174);
    if(surface) {surface->lpVtbl->Release(surface); GFX_P(LPDIRECTDRAWSURFACE,0xe39174)=0;}
    surface=GFX_P(LPDIRECTDRAWSURFACE,0xe39178);
    if(surface) {surface->lpVtbl->Release(surface); GFX_P(LPDIRECTDRAWSURFACE,0xe39178)=0;}
    GFX_P(void *,0xe92e18)=0;
    draw=GFX_P(LPDIRECTDRAW,0xe39148);
    if(release_device&&draw) {
        draw->lpVtbl->Release(draw); GFX_P(LPDIRECTDRAW,0xe39148)=0;
        FUN_004ce6e0(s_OK__DD_Released__005d1d00,0,0);
    }
    return 1;
}
