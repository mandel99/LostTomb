#include "graphics_session.h"
#include <stdio.h>
#include <string.h>
#define s_ERR_DD_Enable_error_level__005d1cb8 (*(char (*)[27])0x005d1cb8u)
#define s_ERR_DD_Not_setCooperative_Level___005d1c94 (*(char (*)[34])0x005d1c94u)
#define s_ERR_DD_SetMode_failed_err___005d1c58 (*(char (*)[28])0x005d1c58u)
#define s_OK__DD_256_cols_switching_to_ful_005d1cd4 (*(char (*)[42])0x005d1cd4u)
#define s_OK__DD_Kept_resolution__005d1c28 (*(char (*)[24])0x005d1c28u)
#define s_OK__DD_SetCooperative_Level__005d1c74 (*(char (*)[29])0x005d1c74u)
#define s_OK__DD_Set_resolution__005d1c40 (*(char (*)[23])0x005d1c40u)

extern void FUN_004ce6e0(char *,int,int);
extern void FUN_004e2970(int,int,int);
extern void FUN_004cfdc0(uint32_t);
extern void FUN_00414bd0(void);
extern void FUN_004e2ab0(void);
int FUN_00426b40(void)
{
    const RestoreGraphicsApi *api=restore_graphics_platform();
    HDC dc=api->get_dc(0);
    LPDIRECTDRAW draw;
    HRESULT result;
    HWND window;
    RECT rect,work={0};
    LONG style;
    int planes,bits,x,y;
    api->device_caps(dc,NUMCOLORS);
    planes=api->device_caps(dc,PLANES); bits=api->device_caps(dc,BITSPIXEL);
    GFX_U(0xe38e58)=(uint32_t)planes*(uint32_t)bits;
    api->release_dc(0,dc);
    GFX_I(0xe38db8)=api->metrics(SM_CXSCREEN);
    GFX_I(0xe38db4)=api->metrics(SM_CYSCREEN);
    if(GFX_I(0xe38db8)<GFX_I(0xe38db0)) {
        if(!GFX_U(0xe38e5c)) FUN_004e2970(0x39,0,0);
        GFX_U(0xe38e5c)=1;
    } else if(!GFX_U(0xe38e5c)&&GFX_U(0xe38e58)!=16) {
        FUN_004e2970(0x39,0,0);
        FUN_004ce6e0(s_OK__DD_256_cols_switching_to_ful_005d1cd4,0,0);
        GFX_U(0xe38e5c)=1;
    }
    if(GFX_U(0xe38e5c)) api->palette_entries(0,0,256,(PALETTEENTRY *)0x70d508u);
    draw=GFX_P(LPDIRECTDRAW,0xe39148);
    if(!draw) {
        result=api->create_draw(0,&draw,0);
        if(result!=DD_OK) {FUN_004ce6e0(s_ERR_DD_Enable_error_level__005d1cb8,0,result); return 0;}
    }
    if(!GFX_P(LPDIRECTDRAW,0xe39148)) {
        result=draw->lpVtbl->SetCooperativeLevel(draw,GFX_P(HWND,0xe39140),GFX_U(0xe38e5c)?0x13:8);
        if(result!=DD_OK) {FUN_004ce6e0(s_ERR_DD_Not_setCooperative_Level___005d1c94,0,result); return 0;}
        FUN_004ce6e0(s_OK__DD_SetCooperative_Level__005d1c74,0,0);
    }
    window=GFX_P(HWND,0xe39140);
    style=api->window_long(window,GWL_STYLE);
    if(GFX_U(0xe38e5c)) {
        api->set_window_long(window,GWL_STYLE,(LONG)(((uint32_t)style&0xff3bffffu)|0x90000000u));
        result=draw->lpVtbl->SetDisplayMode(draw,GFX_U(0xe38d9c),GFX_U(0xe38d98),16);
        if(result!=DD_OK) {
            FUN_004cfdc0(2); FUN_00414bd0(); FUN_004e2ab0();
            result=draw->lpVtbl->SetDisplayMode(draw,GFX_U(0xe38d9c),GFX_U(0xe38d98),16);
            if(result!=DD_OK) {
                FUN_004cfdc0(1); FUN_00414bd0(); FUN_004e2ab0();
                result=draw->lpVtbl->SetDisplayMode(draw,GFX_U(0xe38d9c),GFX_U(0xe38d98),16);
                if(result!=DD_OK) {FUN_004ce6e0(s_ERR_DD_SetMode_failed_err___005d1c58,0,result); return 0;}
            }
        }
        FUN_004ce6e0(s_OK__DD_Set_resolution__005d1c40,0,0);
    } else {
        DWORD extended,adjust_style;
        HMENU menu;
        api->set_window_long(window,GWL_STYLE,(LONG)(((uint32_t)style&0x7f30ffffu)|0xcf0000u));
        rect.left=rect.top=0; rect.right=GFX_I(0xe38d9c); rect.bottom=GFX_I(0xe38d98);
        extended=(DWORD)api->window_long(window,GWL_EXSTYLE);
        menu=api->menu(window); adjust_style=(DWORD)api->window_long(window,GWL_STYLE);
        api->adjust(&rect,adjust_style,menu!=0,extended);
        x=(int32_t)(GFX_U(0xe38db8)-GFX_U(0xe38db0))/2;
        y=(int32_t)(GFX_U(0xe38db4)-GFX_U(0xe38dac))/2;
        GFX_I(0xe390e8)=x; GFX_I(0xe390e4)=y;
        api->position(window,0,0,0,(int32_t)((uint32_t)rect.right-(uint32_t)rect.left),
                      (int32_t)((uint32_t)rect.bottom-(uint32_t)rect.top),0x16);
        api->position(window,HWND_NOTOPMOST,0,0,0,0,0x13);
        api->parameters(SPI_GETWORKAREA,0,&work,0);
        api->window_rect(window,&rect);
        if(rect.left<work.left) rect.left=work.left;
        if(rect.top<work.top) rect.top=work.top;
        /* The native routine still uses its computed center, not rect.left/top. */
        api->position(window,0,x,y,0,0,0x15);
        FUN_004ce6e0(s_OK__DD_Kept_resolution__005d1c28,0,0);
    }
    GFX_P(LPDIRECTDRAW,0xe39148)=draw;
    return 1;
}
