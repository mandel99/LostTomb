#include "sprite_render.h"
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
extern void FUN_004d6480(uint32_t,int32_t,int32_t,int32_t,int32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
/* Whole5208F0. Cache the sidebar edge, reload live hover/submenu state at
   native call sites, and index the submenu label table in DWORDs exactly once. */
void FUN_005208f0(void)
{
    uint32_t edge=U(0xf53f64u),x,index,dy=0,state,style;
    if(U(0xf53f78u)==1u)edge=U(0xf53f60u);
    x=edge-170u;
    for(index=0;index<10u;++index,dy+=24u){
        style=U(0x7056c8u)!=index+1u?2u:1u;
        if(I(U(0x7056dcu))>0)style=2;
        FUN_004d6480(6,I(x),I(U(0x7056e8u)+dy+34u),10,1,style,I(U(0xe38da8u)),I(U(0xe38da4u)));
        FUN_004d6250(14,U(0x5cf734u+index*4u),I(x),I(U(0x7056e8u)+dy+37u),160,U(0xf56f8cu),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
    }
    if(I(U(0x7056dcu))<=0)return;
    (void)FUN_0053e590(I(U(0xf5690cu)),I(edge-185u),I(U(0x7056e8u)+U(0x7056d0u)*24u+40u));
    if(I(U(0x7056d8u))<=0)return;
    x=edge-348u;index=0;
    do{
        style=U(0x7056c4u)!=index+1u?2u:1u;
        FUN_004d6480(6,I(x),I(U(0x7056e8u)+(U(0x7056d0u)+index)*24u+34u),10,1,style,I(U(0xe38da8u)),I(U(0xe38da4u)));
        state=U(0x7056dcu);
        FUN_004d6250(14,U(0x5cf75cu+(index+state*30u)*4u),I(x),I(U(0x7056e8u)+(U(0x7056d0u)+index)*24u+37u),160,U(0xf56f8cu),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
        ++index;
    }while(I(index)<I(U(0x7056d8u)));
}
