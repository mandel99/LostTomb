#include "graphics_session.h"
#include <stdio.h>
#include <string.h>
#define s_ERR_gfxBegin_failed__005e088c (*(char (*)[21])0x005e088cu)

extern void FUN_004ce6e0(char *,int,int);
int FUN_004d0220(void)
{
    GFX_U(0x5e06b0)=1; FUN_004d01e0(0);
    GFX_U(0xe390e0)=FUN_004d0060();
    if(!GFX_U(0xe390e0)) {FUN_004ce6e0(s_ERR_gfxBegin_failed__005e088c,0,0); return 0;}
    FUN_00426f60(); /* Original ignores the clear result here. */
    GFX_U(0x5e06b0)=0; GFX_U(0xe3912c)=1; GFX_B(0xe38dca)=1;
    return 1;
}
