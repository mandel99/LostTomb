#include "graphics_session.h"
#include <stdio.h>
#include <string.h>
#define s_Back_Buffer___x_005e07dc (*(char (*)[18])0x005e07dcu)
#define s_ERR_GFX_DirectDraw_not_enabled__005e07bc (*(char (*)[32])0x005e07bcu)
#define s_ERR_GFX_couldn_t_create_flipping_005e0844 (*(char (*)[42])0x005e0844u)
#define s_ERR_Trying_default_display_mode_005e0824 (*(char (*)[32])0x005e0824u)
#define s_Front_Buffer___x_005e07f0 (*(char (*)[18])0x005e07f0u)
#define s_OK__GFX_DirectDraw_enabled__005e0870 (*(char (*)[28])0x005e0870u)
#define s_Old_res__d__d__Color_mode__d_005e0804 (*(char (*)[29])0x005e0804u)

extern void FUN_004ce6e0(char *,int,int);
extern void FUN_004cfd60(int);
int FUN_004d0060(void)
{
    char message[100];
    const RestoreGraphicsApi *api=restore_graphics_platform();
    if(FUN_00426b40()!=1) {FUN_004ce6e0(s_ERR_GFX_DirectDraw_not_enabled__005e07bc,0,0); return 0;}
    FUN_004ce6e0(s_OK__GFX_DirectDraw_enabled__005e0870,0,0);
    if(FUN_00426ff0()!=1) {
        FUN_004ce6e0(s_ERR_GFX_couldn_t_create_flipping_005e0844,0,0);
        FUN_004ce6e0(s_ERR_Trying_default_display_mode_005e0824,0,0);
        FUN_00426ee0(1);
        GFX_U(0xe38e68)=GFX_U(0xe92e4c); GFX_U(0xe38e5c)=GFX_U(0x5e06d8);
        FUN_004cfd60((int)GFX_U(0xe92e4c));
        snprintf(message,sizeof(message),s_Old_res__d__d__Color_mode__d_005e0804,
                 GFX_U(0xe38db0),GFX_U(0xe38dac),GFX_U(0xe92dfc));
        FUN_004ce6e0(message,0,0);
        if(FUN_00426b40()!=1||FUN_00426ff0()!=1) {FUN_00426ee0(1); return 0;}
    }
    /* Original ECX receiver, evidenced by 004d0151. */
    FUN_004d0480(RESTORE_SURFACE_CONTEXT,GFX_P(LPDIRECTDRAWSURFACE,0xe39178),GFX_P(LPDIRECTDRAWSURFACE,0xe39174));
    snprintf(message,sizeof(message),s_Front_Buffer___x_005e07f0,GFX_U(0xe39178)); api->debug(message);
    snprintf(message,sizeof(message),s_Back_Buffer___x_005e07dc,GFX_U(0xe39174)); api->debug(message);
    GFX_U(0xe92e4c)=GFX_U(0xe38e68); GFX_U(0x5e06d8)=GFX_U(0xe38e5c); GFX_B(0x5e06d4)=0;
    return 1;
}
