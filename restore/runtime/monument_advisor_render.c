#include "monument_advisor_render.h"
#include "sprite_render.h"
#include <stdint.h>
extern int32_t FUN_0040cc40(int32_t excluded);
extern void FUN_004cd680(int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d65b0(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
static int32_t x(uint32_t dx){return I(U(0xf53f0cu)+dx);}
static int32_t y(uint32_t dy){return I(U(0xf53f08u)+dy);}

/* Whole52B520. The real36-slot resource query runs after the heading;
   capture no cursor/origin across it that the original reloads afterward. */
void FUN_0052b520(void)
{
    uint32_t phase=5u-U(0xf53f48u),item,dx,dy,font;
    U(0xf53f0cu)=U(0xe38da8u);U(0xf53f08u)=U(0xe38da4u);
    U(0xf53f04u)=40;U(0xf53f00u)=27;
    if(I(phase)>=5)return;
    if(I(phase)>0){
        FUN_004cd680(x(phase*16u),y(phase*8u),I((40u-phase*2u)*16u),I((27u-phase*2u)*16u),0);
        return;
    }
    FUN_00536820(I(U(0xe38da8u)),I(U(0xe38da4u)),40,27);
    FUN_004d65b0(x(32),y(53),36,14,I(U(0xe38da8u)),I(U(0xe38da4u)));
    (void)FUN_0053e590(I(U(0xf5681cu)+12u),x(10),y(10));
    U(0xe20778u)=0;
    FUN_004d62f0(199,0,x(60),y(12),U(0xf56fb4u),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
    if(FUN_0040cc40(0)){item=3;dx=U(0xe20778u)+60u;dy=30;font=0xf56f8cu;}
    else{item=12;dx=84;dy=140;font=0xf56f9cu;}
    FUN_004d62f0(199,item,x(dx),y(dy),U(font),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
}
