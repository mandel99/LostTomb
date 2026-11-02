#include "actor_render.h"
#include "actor_coordinates.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

/* Complete native43E1A0. Cost, font token, clipping bounds, coordinates and
   player are DWORDs. Truncating the font selects unrelated sprite glyphs. */
void FUN_0043e1a0(void)
{
    int32_t x,y;
    uint32_t price,cash,color,screen_y;
    if(!U(0xbca300u)||U(0xe38dccu)||!U(0xbc9724u))return;
    FUN_004f0ba0((int32_t)U(0xbca304u),(int32_t)U(0xbca308u),&x,&y);
    U(0xd3b55cu)=U(0x70e43cu);U(0xd3b558u)=U(0x70e438u);
    U(0xd3b554u)=U(0x70e434u);U(0xd3b550u)=U(0x70e430u);
    screen_y=(uint32_t)y+U(0x70e44cu);
    price=U(0xbc9724u);cash=U(0xea187cu+U(0xeaac28u)*0x49d8u);
    color=(int32_t)price<=(int32_t)cash?0xfac1u:0xf800u;
    FUN_004cce00(price,0x40,(const char *)(uintptr_t)0x57c048u,
        U(0x70e458u)+(uint32_t)x+1u,screen_y-U(0x70e44cu)+1u,U(0xf56f84u),0);
    FUN_004cce00(U(0xbc9724u),0x40,(const char *)(uintptr_t)0x57c048u,
        U(0x70e458u)+(uint32_t)x,screen_y-U(0x70e44cu),U(0xf56f84u),color);
    U(0xbc9724u)=0;U(0xd3b55cu)=0;U(0xd3b558u)=U(0xe38db0u);
    U(0xd3b554u)=0;U(0xd3b550u)=U(0xe38dacu);
}
