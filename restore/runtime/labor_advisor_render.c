#include "labor_advisor_render.h"
#include "actor_predicate.h"
#include "menu_training_render.h"
#include "menu_tail_ui.h"
#include "sprite_render.h"
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6860(int32_t,uint32_t,const char *,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
#include "pixel_access.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
static int32_t x(uint32_t dx){return I(U(0xf53f0cu)+dx);}
static int32_t y(uint32_t dy){return I(U(0xf53f08u)+dy);}
static int32_t value(uint32_t field){return I(U(field+U(0xeaac28u)*0x49d8u));}
static void label(uint32_t item,uint32_t dx,uint32_t dy,uint32_t font)
{FUN_004d62f0(0x32,item,x(dx),y(dy),font,0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void number(int32_t amount,uint32_t suffix,uint32_t dx,uint32_t dy,uint32_t font)
{FUN_004d6860(amount,0x40,(const char *)(uintptr_t)suffix,x(dx),y(dy),font,0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void centered(uint32_t group,uint32_t item,uint32_t dx,uint32_t dy,uint32_t width,uint32_t font)
{FUN_004d6250(group,item,x(dx),y(dy),width,font,0,I(U(0xe38da8u)),I(U(0xe38da4u)));}

/* Whole4C38F0: WORD existence flag,49D8 player stride, full first query and
   AL second query. Reload the player/building for the second native call. */
int32_t FUN_004c38f0(void)
{
    uint32_t offset=U(0xeaac28u)*0x49d8u;
    if(!H(0xea5ce8u+offset))return 0;
    if(FUN_004c5120(I(U(0xea5f9cu+offset)))!=1u)return 0;
    offset=U(0xeaac28u)*0x49d8u;
    return FUN_004c52d0(I(U(0xea5f9cu+offset)),2u)?-2:0;
}

/* Whole521080, including the opening transition and live text cursor. */
void FUN_00521080(void)
{
    int32_t bonus=FUN_004c38f0();uint32_t phase=5u-U(0xf53f48u);
    U(0xf53f0cu)=U(0xe38da8u);U(0xf53f08u)=U(0xe38da4u);
    U(0xf53f04u)=40;U(0xf53f00u)=26;
    if(I(phase)>=5)return;
    if(I(phase)>0){
        FUN_004cd680(I(U(0xe38da8u)+phase*16u),I(U(0xe38da4u)+phase*8u),
                     I((40u-phase*2u)*16u),I((26u-phase*2u)*16u),0);return;
    }
    FUN_00536820(I(U(0xe38da8u)),I(U(0xe38da4u)),40,26);
    (void)FUN_0053e590(I(U(0xf5681cu)),x(10),y(10));
    label(0,60,12,U(0xf56fb4u));
    label(21,60,56,U(0xf56f8cu));label(22,170,56,U(0xf56f8cu));
    label(23,400,56,U(0xf56f8cu));label(24,500,56,U(0xf56f8cu));
    U(0xe20778u)=0;number(value(0xea4464u),0x57c048u,32,320,U(0xf56f8cu));
    label(12,U(0xe20778u)+32u,320,U(0xf56f8cu));
    number(value(0xea4468u),0x57c048u,U(0xe20778u)+50u,320,U(0xf56f8cu));
    label(13,U(0xe20778u)+50u,320,U(0xf56f8cu));
    number(value(0xea446cu),0x5f89d4u,U(0xe20778u)+50u,320,U(0xf56f8cu));
    FUN_004d65b0(x(64),y(350),32,2,I(U(0xe38da8u)),I(U(0xe38da4u)));
    U(0xe20778u)=0;label(14,70,359,U(0xf56f9cu));
    U(0xe20778u)=0;number(value(0xea4478u),0x57c048u,230,359,U(0xf56f9cu));
    label(15,U(0xe20778u)+230u,359,U(0xf56f9cu));
    label(18,U(0xe20778u)+230u,359,U(0xf56f9cu));
    number(I((uint32_t)value(0xea447cu)+(uint32_t)bonus),0x5f89d0u,
           U(0xe20778u)+230u,359,U(0xf56f9cu));
    U(0xe20778u)=0;label(19,64,390,U(0xf56f8cu));
    number(value(0xea5f10u),0x5e03ecu,U(0xe20778u)+64u,390,U(0xf56f8cu));
}

/* Whole5215D0: nine20-byte category records, real highlight source and
   required/filled counts, including the distinct unmet-demand font. */
void FUN_005215d0(void)
{
    uint32_t index,offset,row,selected,font;int32_t filled;
    if(I(U(0xf53f48u))<=5)return;
    FUN_004d65b0(x(32),y(70),36,15,I(U(0xe38da8u)),I(U(0xe38da4u)));
    for(index=0,offset=0,row=0;index<9u;++index,offset+=20u,row+=25u){
        selected=B(0xe38ddau)==1?U(0x7056c8u)-1u:U(0x605988u);
        FUN_004d63d0(0,(uint32_t)x(40),(uint32_t)y(row+77u),560,22,
                     selected==index,U(0xe38da8u),U(0xe38da4u));
        if(value(0xea43acu+offset)!=0){
            (void)FUN_0053e590(I(U(0xf5670cu)),x(70),y(row+80u));
            number(value(0xea43acu+offset),0x57c048u,90,row+82u,U(0xf56f9cu));
        }
        U(0xe20778u)=0;label(index+1u,170,row+82u,U(0xf56f9cu));
        number(value(0xea439cu+offset),0x57c048u,410,row+82u,U(0xf56f9cu));
        filled=value(0xea43a0u+offset);font=U(0xf56f9cu);
        if(filled!=value(0xea439cu+offset))font=U(0xf56fa4u);
        number(filled,0x57c048u,510,row+82u,font);
    }
}

/* Whole521800: priority popup and its nine thresholds plus reset row. */
void FUN_00521800(void)
{
    uint32_t index,dx;
    FUN_00536820(x(160),y(176),20,9);
    centered(0x32,25,160,185,320,U(0xf56fb4u));
    for(index=0,dx=0;index<9u;++index,dx+=32u){
        FUN_004cd680(x(dx+178u),y(221),27,27,0);
        centered(0x32,index+27u,dx+178u,224,27,U(0xf56fb4u));
        if(I(U(0x605984u))<=I(index))FUN_004cf090(x(dx+179u),y(222),25,25,1);
    }
    FUN_004cd680(x(180),y(256),280,25,0);
    centered(0x32,26,180,263,280,U(0xf56f8cu));
    centered(13,3,160,296,320,U(0xf56f8cu));
}

/* Whole5219C0: callback changes to the full selection are read per box. */
void FUN_005219c0(void)
{
    uint32_t index,dx;
    for(index=0,dx=0;index<9u;++index,dx+=32u)
        FUN_004cd680(x(dx+178u),y(221),27,27,U(0x7056c8u)-2u==index?0xf800:0);
    FUN_004cd680(x(180),y(256),280,25,U(0x7056c8u)==1u?0xf800:0);
}
