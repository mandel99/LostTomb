#include "minimap.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
void restore_minimap_cell(RestoreSpriteLibraries *self,int32_t image,int32_t x,int32_t y)
{
    uintptr_t manager=(uintptr_t)self;
    if(image>0&&image<25500) {
        if(image>=(int32_t)U(manager+0x30f5ccu))image=0;
        U(0xd3b5b8)=U(manager+0x180e48u+(uint32_t)image*64u);
        if((int32_t)U(0xd3b5b8)>0) {
            U(0xd3b5e0)=(uint32_t)x;U(0xd3b5dc)=(uint32_t)y;
            FUN_0040fbe0((const void *)(uintptr_t)U(0xd3b5b8));
        }
    }
}
/* Both original callers bind ECX to this same global sprite manager. */
void FUN_0053f4a0(int32_t image,int32_t x,int32_t y)
{restore_minimap_cell(RESTORE_SPRITE_LIBRARIES,image,x,y);}
