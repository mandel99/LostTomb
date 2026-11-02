#include "overlay_footprints.h"

/* Whole432620: flatten only the cliff terrain mask; preserve DWORD tile,
   image and scratch state and byte-sized footprint cells. */
void FUN_00432620(int32_t x,int32_t y)
{
    uint32_t tile=GR_U(0x70e488u),f,image,px,py,count,table;
    if((GR_U(0xaf598cu+tile*4u)&0x20000002u)==0x20000002u){
        f=GR_B(0xb285ccu+tile)&15u;
        GR_U(0xd3b5c0u)=GR_U(0xf56d14u);
        table=overlay_pairs(f<=2u?f:0u,&count);
        overlay_draw_pairs(table,count,0,(uint32_t)x,(uint32_t)y,0,1);
        return;
    }
    f=GR_B(0xb285ccu+tile)&15u;image=GR_U(0xb7482cu+tile*4u);
    GR_U(0xd3b5c0u)=image;
    if(ground_footprint(f,(uint32_t)x,(uint32_t)y,&px,&py))
        (void)FUN_0053e590(GR_S(image),GR_S(px),GR_S(py));
}
