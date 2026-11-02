#include "graphics_session.h"
#include <stdio.h>
#include <string.h>

void FUN_004d03f0(void)
{
    RECT destination=*(RECT *)0xe39130u;
    RECT source={0,0,GFX_I(0xe38db0),GFX_I(0xe38dac)};
    LPDIRECTDRAWSURFACE front=GFX_P(LPDIRECTDRAWSURFACE,0xe39178);
    LPDIRECTDRAWSURFACE back=GFX_P(LPDIRECTDRAWSURFACE,0xe39174);
    if(!front||!back) return;
    if(!GFX_U(0xe38e5c)) front->lpVtbl->Blt(front,&destination,back,&source,0,0);
    else front->lpVtbl->BltFast(front,0,0,back,(RECT *)0xe39130u,0);
}
