#include "graphics_session.h"
#include <stdio.h>
#include <string.h>

void FUN_004d01e0(uint32_t release_device)
{
    IUnknown *unknown=GFX_P(IUnknown *,0xe92e00);
    if(unknown) {unknown->lpVtbl->Release(unknown); GFX_P(IUnknown *,0xe92e00)=0;}
    if(GFX_U(0xe390e0)) {FUN_00426ee0((int)release_device); GFX_U(0xe390e0)=0;}
}
