#include "graphics_session.h"
#include <stdio.h>
#include <string.h>
#define s_ERR_DD_Cant_attach_clipper_to_fr_005d1e5c (*(char (*)[44])0x005d1e5cu)
#define s_ERR_DD_Cant_create_W__backbuffer_005d1ecc (*(char (*)[35])0x005d1eccu)
#define s_ERR_DD_Cant_create_clipper__005d1eb0 (*(char (*)[28])0x005d1eb0u)
#define s_ERR_DD_Cant_set_clipper_window_h_005d1e88 (*(char (*)[39])0x005d1e88u)
#define s_ERR_DD_CreateSurface_failed2__005d1f9c (*(char (*)[30])0x005d1f9cu)
#define s_ERR_DD_CreateSurface_failed3__005d1ef0 (*(char (*)[30])0x005d1ef0u)
#define s_ERR_DD_surface_lock_error__005d1f5c (*(char (*)[27])0x005d1f5cu)
#define s_ERR_DD_unsupported_RGB_format__t_005d1d68 (*(char (*)[52])0x005d1d68u)
#define s_ERR__16_bit_surface_format_chang_005d1d34 (*(char (*)[49])0x005d1d34u)
#define s_OK__DD_Creating_one_Vram_and_one_005d1f10 (*(char (*)[49])0x005d1f10u)
#define s_OK__DD_Creating_one_Vram_only__005d1fbc (*(char (*)[31])0x005d1fbcu)
#define s_OK__DD_Surface_created__005d1f44 (*(char (*)[24])0x005d1f44u)
#define s_OK__DD_Surfaces_created_and_clip_005d1e2c (*(char (*)[46])0x005d1e2cu)
#define s_OK__DD_can_lock_primary_surface_005d1f78 (*(char (*)[33])0x005d1f78u)
#define s_OK__DD_surface_format_is_RGB_555_005d1d9c (*(char (*)[34])0x005d1d9cu)
#define s_OK__DD_surface_format_is_RGB_556_005d1dc0 (*(char (*)[34])0x005d1dc0u)
#define s_OK__DD_surface_format_is_RGB_565_005d1de4 (*(char (*)[34])0x005d1de4u)
#define s_OK__DD_surface_format_is_RGB_655_005d1e08 (*(char (*)[34])0x005d1e08u)

extern void FUN_004ce6e0(char *,int,int);
extern int FUN_00427510(int);
int FUN_00426ff0(void)
{
    DDSURFACEDESC desc={0};
    LPDIRECTDRAW draw=GFX_P(LPDIRECTDRAW,0xe39148);
    LPDIRECTDRAWSURFACE surface;
    HRESULT result;
    uint32_t previous=GFX_U(0xe92dfc);
    int windowed=!GFX_U(0xe38e5c);
    char message[100];
    FUN_004ce6e0(windowed?s_OK__DD_Creating_one_Vram_and_one_005d1f10:s_OK__DD_Creating_one_Vram_only__005d1fbc,0,0);
    desc.dwSize=sizeof(desc); desc.dwFlags=DDSD_CAPS; desc.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE;
    result=draw->lpVtbl->CreateSurface(draw,&desc,(LPDIRECTDRAWSURFACE *)0xe39178u,0);
    if(result!=DD_OK) {
        FUN_004ce6e0(windowed?s_ERR_DD_CreateSurface_failed3__005d1ef0:s_ERR_DD_CreateSurface_failed2__005d1f9c,0,result);
        return 0;
    }
    memset(&desc,0,sizeof(desc)); desc.dwSize=sizeof(desc);
    GFX_U(0xe92e04)=0;
    surface=GFX_P(LPDIRECTDRAWSURFACE,0xe39178);
    result=surface->lpVtbl->Lock(surface,0,&desc,DDLOCK_WAIT,0);
    if(result==DD_OK) {
        FUN_004ce6e0(s_OK__DD_can_lock_primary_surface_005d1f78,0,0);
        GFX_U(0xe92e04)=0; surface=GFX_P(LPDIRECTDRAWSURFACE,0xe39178);
        surface->lpVtbl->Unlock(surface,0);
    } else FUN_004ce6e0(s_ERR_DD_surface_lock_error__005d1f5c,0,FUN_00427510(result)+1);
    surface=FUN_00427480(GFX_U(0xe38d9c),GFX_U(0xe38d98),1,0);
    GFX_P(LPDIRECTDRAWSURFACE,0xe39174)=surface;
    if(!surface) {FUN_004ce6e0(s_ERR_DD_Cant_create_W__backbuffer_005d1ecc,0,0); return 0;}
    if(surface->lpVtbl->Lock(surface,0,&desc,DDLOCK_WAIT,0)==DD_OK) {
        GFX_P(void *,0xe92e18)=desc.lpSurface;
        surface=GFX_P(LPDIRECTDRAWSURFACE,0xe39174); surface->lpVtbl->Unlock(surface,0);
    }
    if(windowed) {
        LPDIRECTDRAWCLIPPER clipper;
        draw=GFX_P(LPDIRECTDRAW,0xe39148);
        if(draw->lpVtbl->CreateClipper(draw,0,(LPDIRECTDRAWCLIPPER *)0xe39170u,0)!=DD_OK) {
            FUN_004ce6e0(s_ERR_DD_Cant_create_clipper__005d1eb0,0,0); return 0;
        }
        clipper=GFX_P(LPDIRECTDRAWCLIPPER,0xe39170);
        if(clipper->lpVtbl->SetHWnd(clipper,0,GFX_P(HWND,0xe39140))!=DD_OK) {
            FUN_004ce6e0(s_ERR_DD_Cant_set_clipper_window_h_005d1e88,0,0); return 0;
        }
        surface=GFX_P(LPDIRECTDRAWSURFACE,0xe39178);
        if(surface->lpVtbl->SetClipper(surface,GFX_P(LPDIRECTDRAWCLIPPER,0xe39170))!=DD_OK) {
            FUN_004ce6e0(s_ERR_DD_Cant_attach_clipper_to_fr_005d1e5c,0,0); return 0;
        }
    }
    FUN_004ce6e0(windowed?s_OK__DD_Surfaces_created_and_clip_005d1e2c:s_OK__DD_Surface_created__005d1f44,0,0);
    desc.dwSize=sizeof(desc); desc.dwFlags=DDSD_PIXELFORMAT; desc.ddpfPixelFormat.dwSize=sizeof(DDPIXELFORMAT);
    surface=GFX_P(LPDIRECTDRAWSURFACE,0xe39178);
    surface->lpVtbl->GetSurfaceDesc(surface,&desc);
    if(desc.ddpfPixelFormat.dwRBitMask==0xfc00) {
        GFX_U(0xe3914c)=0x655; GFX_U(0xe92dfc)=3;
        FUN_004ce6e0(s_OK__DD_surface_format_is_RGB_655_005d1e08,0,0);
    } else if(desc.ddpfPixelFormat.dwGBitMask==0x7e0) {
        GFX_U(0xe3914c)=0x565; GFX_U(0xe92dfc)=2;
    } else if(desc.ddpfPixelFormat.dwBBitMask==0x3f) {
        GFX_U(0xe3914c)=0x556; GFX_U(0xe92dfc)=1;
    } else {GFX_U(0xe3914c)=0x555; GFX_U(0xe92dfc)=0;}
    if(GFX_U(0xe3914c)==0x565) FUN_004ce6e0(s_OK__DD_surface_format_is_RGB_565_005d1de4,0,0);
    if(GFX_U(0xe3914c)==0x556) FUN_004ce6e0(s_OK__DD_surface_format_is_RGB_556_005d1dc0,0,0);
    if(GFX_U(0xe3914c)==0x555) FUN_004ce6e0(s_OK__DD_surface_format_is_RGB_555_005d1d9c,0,0);
    if(GFX_U(0xe92dfc)==1||GFX_U(0xe92dfc)==3) {
        FUN_004ce6e0(s_ERR_DD_unsupported_RGB_format__t_005d1d68,0,0);
        GFX_U(0xe3914c)=0x565; GFX_U(0xe92dfc)=2;
    }
    if(GFX_B(0xe38dda)&&GFX_U(0xe92dfc)!=previous) {
        snprintf(message,sizeof(message),s_ERR__16_bit_surface_format_chang_005d1d34,previous,GFX_U(0xe92dfc));
        FUN_004ce6e0(message,0,0); GFX_U(0xe92dfc)=previous;
    }
    return 1;
}
