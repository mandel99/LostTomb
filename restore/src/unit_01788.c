#include "graphics_session.h"
#include <stdio.h>
#include <string.h>
#define s_Back_Surface_Ptr___x_005e08a4 (*(char (*)[22])0x005e08a4u)
#define s_Front_Surface_Ptr___x_005e08d8 (*(char (*)[23])0x005e08d8u)
#define s_Unable_to_lock_bg_surface_005e08bc (*(char (*)[27])0x005e08bcu)
#define s_Unable_to_lock_fg_surface_005e08f0 (*(char (*)[27])0x005e08f0u)

int FUN_004d0480(RestoreSurfaceContext *self,LPDIRECTDRAWSURFACE front,LPDIRECTDRAWSURFACE back)
{
    DDSURFACEDESC desc={0};
    char message[100];
    const RestoreGraphicsApi *api=restore_graphics_platform();
    self->front=front; self->back=back; desc.dwSize=sizeof(desc);
    if(front->lpVtbl->Lock(front,0,&desc,DDLOCK_WAIT,0)==DD_OK) {
        self->front_pixels=desc.lpSurface; front->lpVtbl->Unlock(front,0);
        snprintf(message,sizeof(message),s_Front_Surface_Ptr___x_005e08d8,(uint32_t)(uintptr_t)self->front_pixels);
        api->debug(message);
    } else api->debug(s_Unable_to_lock_fg_surface_005e08f0);
    memset(&desc,0,sizeof(desc)); desc.dwSize=sizeof(desc);
    if(back->lpVtbl->Lock(back,0,&desc,DDLOCK_WAIT,0)==DD_OK) {
        self->back_pixels=desc.lpSurface; back->lpVtbl->Unlock(back,0);
        snprintf(message,sizeof(message),s_Back_Surface_Ptr___x_005e08a4,(uint32_t)(uintptr_t)self->back_pixels);
        api->debug(message);
    } else api->debug(s_Unable_to_lock_bg_surface_005e08bc);
    self->width=(int32_t)desc.dwWidth; self->height=(int32_t)desc.dwHeight;
    if(desc.ddpfPixelFormat.dwRBitMask==0xf800&&desc.ddpfPixelFormat.dwGBitMask==0x7e0&&desc.ddpfPixelFormat.dwBBitMask==0x1f) self->format=1;
    else if(desc.ddpfPixelFormat.dwRBitMask==0x7c00&&desc.ddpfPixelFormat.dwGBitMask==0x3e0&&desc.ddpfPixelFormat.dwBBitMask==0x1f) self->format=0;
    else {self->format=-1; return 0;}
    return 1;
}
