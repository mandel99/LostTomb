#include "graphics_session.h"
#include <stdio.h>
#include <string.h>
#define s_ERR_DD_Clearing_Fill_failed_no__005d1d14 (*(char (*)[32])0x005d1d14u)

extern void FUN_004ce6e0(char *,int,int);
int FUN_00426f60(void)
{
    DDBLTFX effects={0};
    LPDIRECTDRAWSURFACE surface;
    int failed=1;
    effects.dwSize=sizeof(effects); effects.dwFillColor=0;
    restore_graphics_platform()->update(GFX_P(HWND,0xe39140));
    surface=GFX_P(LPDIRECTDRAWSURFACE,0xe39174);
    if(surface->lpVtbl->Blt(surface,0,0,0,0x01000400,&effects)==DD_OK) {
        failed=2; surface=GFX_P(LPDIRECTDRAWSURFACE,0xe39178);
        if(surface->lpVtbl->Blt(surface,0,0,0,0x01000400,&effects)==DD_OK) return 1;
    }
    FUN_004ce6e0(s_ERR_DD_Clearing_Fill_failed_no__005d1d14,0,failed);
    return 0;
}
