#include "ratings_advisor_render.h"
#include "world_render.h"
#include "sprite_render.h"
extern uint32_t FUN_004ea930(void);
extern void FUN_004cd680(int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6160(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d63d0(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d6860(int32_t,uint32_t,const char *,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004cce90(int32_t,uint32_t,const char *,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_004d65b0(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
#define EMPTY ((const char *)(uintptr_t)0x57c048u)
static uint32_t player_offset(void){return U(0xeaac28u)*0x49d8u;}
static uint32_t value(uint32_t field){return U(field+player_offset());}
static int32_t x(uint32_t dx){return I(U(0xf53f0cu)+dx);}
static int32_t y(uint32_t dy){return I(U(0xf53f08u)+dy);}
static void label(uint32_t item,uint32_t dx,uint32_t dy,uint32_t font)
{FUN_004d62f0(0x35,item,x(dx),y(dy),font,0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void number(uint32_t amount,uint32_t dx,uint32_t dy)
{FUN_004d6860(I(amount),0x40,EMPTY,x(dx),y(dy),U(0xf56f8cu),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}

/* Whole4F6AD0/6 instructions. The existing complete4EA280 scanner counts
   the native4000 records starting at ID1; no C# index bound is substituted. */
void FUN_004f6ad0(void){U(0xeadb28u)=(uint32_t)FUN_004ea280(I(U(0x784c38u)));}

/* Whole4F7720/4F7770: native DWORD hover and geometry, including the
   foreground's live reload of selection/origin for each of four buttons. */
void FUN_004f7720(int32_t column)
{
    FUN_004d63d0(0,x((uint32_t)column*120u-40u),y(286),120,66,
                 U(0x7056c8u)==(uint32_t)column,I(U(0xe38da8u)),I(U(0xe38da4u)));
}
void FUN_004f7770(void)
{
    uint32_t phase=5u-U(0xf53f48u),i;
    if(I(phase)>=5||I(phase)>0)return;
    for(i=1;i<=4u;++i)FUN_004f7720(I(i));
}

/* Whole4F77E0: signed WORD bitmap height and full image limit. The middle
   image is cached after drawing the base; the cap reloads base and limit. */
void FUN_004f77e0(int32_t px,int32_t py,int32_t amount,int32_t complete)
{
    uint32_t image=U(0xf56a04u),index,top,middle,count,i;
    index=I(image)<I(U(0x126517cu))?image:0u;
    top=(uint32_t)py-(uint32_t)(int32_t)H(0x10d6a0eu+index*64u);
    (void)FUN_0053e590(I(image),px,I(top));
    middle=U(0xf56a04u)+1u;
    count=(uint32_t)amount+(uint32_t)(amount/2);
    if(I(count)>0)for(i=0;i<count;++i){
        --top;(void)FUN_0053e590(I(middle),I((uint32_t)px+15u),I(top));
    }
    if(complete){
        image=U(0xf56a04u)+2u;index=I(image)<I(U(0x126517cu))?image:0u;
        top-=(uint32_t)(int32_t)H(0x10d6a0eu+index*64u);
        (void)FUN_0053e590(I(image),I((uint32_t)px-2u),I(top));
    }
}

static void column(uint32_t index)
{
    uint32_t rating=value(0xea5b3cu+index*4u),left=(uint32_t)x(80u+index*120u);
    uint32_t top=(uint32_t)y(294),font,ox,oy,enabled,required,complete;
    if(index==3u)FUN_004f7720(4);
    else FUN_004d63d0(0,I(left),I(top-8u),120,66,U(0x7056c8u)==index+1u,I(U(0xe38da8u)),I(U(0xe38da4u)));
    ox=U(0xe38da8u);oy=U(0xe38da4u);
    font=index<2u?U(0xf56f8cu):FUN_004381a0(0x27b);
    FUN_004d6250(0x35,index+1u,I(left),I(top),120,font,0,I(ox),I(oy));
    font=index==0u?U(0xf56fb4u):FUN_004381a0(0x280);
    FUN_004cce90(I(rating),0x40,(const char *)(uintptr_t)0x5f76c0u,I(left),I(top+15u),110,font,0);
    enabled=B(0x784c3cu+index);U(0xe20778u)=0;
    ox=U(0xe38da8u);oy=U(0xe38da4u);
    font=index==0u&&!enabled?U(0xf56f8cu):FUN_004381a0(0x27b);
    required=enabled?U(0x784c24u+index*4u):0;
    top+=40u;
    FUN_004d6860(I(required),0x40,(const char *)(uintptr_t)0x7027d4u,I(left+5u),I(top),font,0,I(ox),I(oy));
    ox=U(0xe38da8u);oy=U(0xe38da4u);font=FUN_004381a0(0x27b);
    FUN_004d62f0(0x35,5,I(left+5u+U(0xe20778u)),I(top),font,0,I(ox),I(oy));
    complete=!B(0x784c3cu+index)||I(rating)>=I(U(0x784c24u+index*4u));
    if(index==2u&&!(uint8_t)FUN_004ea930())complete=0;
    if(I(rating)<0)rating=0;else if(I(rating)>100)rating=100;
    FUN_004f77e0(x(110u+index*120u),y(274),I(rating),I(complete));
}

/* Whole4F6AF0. Per-column coordinates/ratings and the advice block are
   cached as in the native body. Origins pushed before a font query remain
   captured across that query; all nine paragraph arguments are explicit. */
void FUN_004f6af0(void)
{
    uint32_t phase=5u-U(0xf53f48u),i,left,top,heading_y,offset,selection;
    uint32_t heading=UINT32_MAX,message=UINT32_MAX,extra=UINT32_MAX,center=0,font,ox,oy;
    U(0xf53f0cu)=U(0xe38da8u);U(0xf53f08u)=U(0xe38da4u);
    U(0xf53f04u)=40;U(0xf53f00u)=27;
    if(I(phase)>=5)return;
    if(I(phase)>0){
        FUN_004cd680(x(phase*16u),y(phase*8u),I((40u-phase*2u)*16u),I((27u-phase*2u)*16u),0);return;
    }
    FUN_00536820(I(U(0xe38da8u)),I(U(0xe38da4u)),40,27);
    (void)FUN_0053e590(I(U(0xf5681cu)+3u),x(10),y(10));
    U(0xe20778u)=0;label(0,60,12,U(0xf56fb4u));
    U(0xe20778u)=0;label(6,280,17,U(0xf56f8cu));
    number(value(0xea1894u),U(0xe20778u)+280u,17);
    if(U(0x784c58u)&&!W(0x784b60u)){
        U(0xe20778u)=0;number(U(0x784c5cu),280,34);
        label(5,U(0xe20778u)+280u,34,U(0xf56f8cu));
    }
    if(U(0x784c34u)&&!W(0x784b60u)){
        U(0xe20778u)=0;
        FUN_004d62f0(0x1d,U(0x784c38u)+40u,x(430),y(17),U(0xf56f8cu),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
        number(U(0xeadb28u),U(0xe20778u)+430u,17);
        U(0xe20778u)=0;number(U(0x784c34u),430,34);
        label(5,U(0xe20778u)+430u,34,U(0xf56f8cu));
    }
    (void)FUN_0053e590(I(U(0xf55bc4u)),x(60),y(48));
    for(i=0;i<4u;++i)column(i);
    FUN_004d65b0(x(64),y(356),32,4,I(U(0xe38da8u)),I(U(0xe38da4u)));
    left=(uint32_t)x(72);top=(uint32_t)y(374);heading_y=(uint32_t)y(359);
    offset=player_offset();selection=U(0xea5ca0u+offset);
    switch(selection){
    case 0:message=8;center=1;top+=6u;break;
    case 1:heading=1;message=I(U(0xea5b3cu+offset))>90?65u:U(0xea5ca4u+offset)+9u;break;
    case 2:
        heading=2;
        if(I(U(0xea5b40u+offset))>90)message=66;
        else{extra=U(0xea5b54u+offset)+20u;if(U(0xea5ca8u+offset))message=U(0xea5ca8u+offset)+20u;}
        break;
    case 3:heading=3;message=I(U(0xea5b44u+offset))>90?67u:U(0xea5cacu+offset)+55u;break;
    case 4:heading=4;message=I(U(0xea5b48u+offset))>90?68u:U(0xea5cb0u+offset)+32u;break;
    default:break;
    }
    U(0xe20778u)=0;
    if(I(heading)>=0)FUN_004d62f0(0x35,heading,I(left),I(heading_y),U(0xf56f9cu),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
    if(I(extra)>=0){
        ox=U(0xe38da8u);oy=U(0xe38da4u);font=FUN_004381a0(0x27d);
        FUN_004d62f0(0x35,extra,x(U(0xe20778u)+72u),y(359),font,0,I(ox),I(oy));
    }
    U(0xe20778u)=0;
    if(I(message)>=0){
        ox=U(0xe38da8u);oy=U(0xe38da4u);font=FUN_004381a0(0x27d);
        if(center)FUN_004d6250(0x35,message,I(left),I(top),496,font,0,I(ox),I(oy));
        else FUN_004d6160(0x35,message,I(left),I(top),496,font,0,I(ox),I(oy));
    }
}
