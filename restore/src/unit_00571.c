#include "placement_preview.h"
#include "footprint.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Whole43DF10:3x3 shoreline preview. Preserve signed treasury comparisons,
   full orientation state and the nine native invalid-tile coordinates. */
void FUN_0043df10(int32_t group)
{
    int32_t sx=0,sy=0,result;
    uint32_t image,orientation,table,x,y;
    FUN_004f0ba0(I(U(0xbca304u)),I(U(0xbca308u)),&sx,&sy);
    sy=I((uint32_t)sy+U(0x70e44cu));
    result=FUN_0047d060(I(U(0xbca304u)),I(U(0xbca308u)),1);
    if(I(U(0xea187cu+U(0xeaac28u)*0x49d8u))<=-5000)result=999;
    else if(result==0){
        orientation=U(0xbc96dcu);
        image=U(0xf55bb4u+(uint32_t)group*8u)+(orientation<=2u?orientation:3u);
        x=(uint32_t)sx;y=(uint32_t)sy-U(0x70e44cu);
        W(0x70e120u)=0x1fe3u;
        (void)FUN_0053e590(I(image),I(x),I(y));
        (void)FUN_0053f250(I(image),I(x),I(y));
        W(0x70e120u)=0;return;
    }
    for(table=0x5cfcf4u;table<0x5cfd3cu;table+=8u){
        x=U(table-4u)+(uint32_t)sx;y=U(table)-U(0x70e44cu)+(uint32_t)sy;
        (void)FUN_0053f4f0(I(U(0xf56be4u)),I(x),I(y),result?0xfffff863u:0x1fe3u);
    }
}
