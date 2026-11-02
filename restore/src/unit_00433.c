#include "graphics_session.h"
#include <stdio.h>
#include <string.h>
#define s_ERR_DD_ExtraSurface_failed__005d1fdc (*(char (*)[28])0x005d1fdcu)

extern void FUN_004ce6e0(char *,int,int);
LPDIRECTDRAWSURFACE FUN_00427480(uint32_t width,uint32_t height,int system_memory,uint32_t unused)
{
    DDSURFACEDESC desc={0};
    LPDIRECTDRAWSURFACE surface=0;
    LPDIRECTDRAW draw=GFX_P(LPDIRECTDRAW,0xe39148);
    (void)unused;
    desc.dwSize=sizeof(desc); desc.dwFlags=DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
    desc.dwWidth=width; desc.dwHeight=height;
    desc.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|(system_memory?DDSCAPS_SYSTEMMEMORY:0);
    if(draw->lpVtbl->CreateSurface(draw,&desc,&surface,0)!=DD_OK) {
        FUN_004ce6e0(s_ERR_DD_ExtraSurface_failed__005d1fdc,0,0); return 0;
    }
    return surface;
}
