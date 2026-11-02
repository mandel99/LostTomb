#include "financial_advisor_render.h"
#include "actor_predicate.h"
#include "sprite_render.h"
extern void FUN_004cd680(int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d65b0(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_004ccc30(uint32_t,uint32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_004d6860(int32_t,uint32_t,const char *,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004cd510(int32_t,int32_t,int32_t,int32_t,uint32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
static uint32_t player_offset(void){return U(0xeaac28u)*0x49d8u;}
static uint32_t value(uint32_t field){return U(field+player_offset());}
static int32_t x(uint32_t dx){return I(U(0xf53f0cu)+dx);}
static int32_t y(uint32_t dy){return I(U(0xf53f08u)+dy);}
static void label(uint32_t item,uint32_t dx,uint32_t dy,uint32_t font)
{FUN_004d62f0(0x3c,item,x(dx),y(dy),U(font),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void number(uint32_t amount,uint32_t suffix,uint32_t dx,uint32_t dy,uint32_t font)
{FUN_004d6860(I(amount),0x40,(const char *)(uintptr_t)suffix,x(dx),y(dy),U(font),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void line(uint32_t dx1,uint32_t dx2,uint32_t dy)
{
    /* Each native line captures its origin once before the real raster call. */
    uint32_t px=U(0xf53f0cu),py=U(0xf53f08u)+dy;
    FUN_004cd510(I(px+dx1),I(py),I(px+dx2),I(py),0);
}

/* Whole52A250,1351 native instructions. Effective tax is cached before
   opening; every ledger/layout field is refreshed at its own native call.
   The compact table retains the complete painter/cursor order. */
void FUN_0052a250(void)
{
    static const struct {uint32_t item,dy,left,right;} rows[]={
        {8,140,0xea44c0u,0xea44bcu},{9,155,0xea44d8u,0xea44d4u},
        {20,170,0xea5cc4u,0xea5cc8u},{24,185,0xea601cu,0xea6020u},
        {10,203,0xea4514u,0xea4518u},{11,227,0xea44e0u,0xea44dcu},
        {12,242,0xea448cu,0xea4488u},{13,257,0xea44f8u,0xea44fcu},
        {14,272,0xea44e8u,0xea44ecu},{15,287,0xea4500u,0xea4504u},
        {16,302,0xea44f0u,0xea44f4u},{21,317,0xea5d54u,0xea5d50u},
        {22,332,0xea5fc4u,0xea5fc8u},{17,350,0xea451cu,0xea4520u},
        {18,373,0xea4524u,0xea4528u},{19,396,0xea452cu,0xea4530u}
    };
    uint32_t offset=player_offset(),rate=U(0x784ddcu),phase,row,font,amount;
    if(W(0xea5ce8u+offset)&&FUN_004c5120(I(U(0xea5f9cu+offset)))==1u){
        rate-=5u;if(I(rate)<0)rate=0;
    }
    phase=5u-U(0xf53f48u);
    U(0xf53f0cu)=U(0xe38da8u);U(0xf53f08u)=U(0xe38da4u);
    U(0xf53f04u)=40;U(0xf53f00u)=27;
    if(I(phase)>=5)return;
    if(I(phase)>0){
        FUN_004cd680(x(phase*16u),y(phase*8u),I((40u-phase*2u)*16u),I((27u-phase*2u)*16u),0);
        return;
    }
    FUN_00536820(I(U(0xe38da8u)),I(U(0xe38da4u)),40,27);
    (void)FUN_0053e590(I(U(0xf5681cu)+10u),x(10),y(10));
    label(0,60,12,0xf56fb4u);
    FUN_004d65b0(x(64),y(48),34,4,I(U(0xe38da8u)),I(U(0xe38da4u)));
    FUN_00536ba0(x(64),y(104),34);
    U(0xe20778u)=0;
    if(I(value(0xea187cu))<0){
        label(3,70,53,0xf56fa4u);amount=0u-value(0xea187cu);font=0xf56fa4u;
    }else{label(2,70,53,0xf56f9cu);amount=value(0xea187cu);font=0xf56f9cu;}
    FUN_004ccc30(8,0,I(amount),x(U(0xe20778u)+72u),y(53),U(font),0);
    label(1,70,76,0xf56f9cu);U(0xe20778u)=0;
    number(value(0xea1878u),0x5e03fcu,240,76,0xf56f9cu);
    label(4,U(0xe20778u)+240u,76,0xf56f9cu);
    FUN_004ccc30(8,0,I(value(0xea5e5cu)),x(U(0xe20778u)+240u),y(76),U(0xf56f9cu),0);
    U(0xe20778u)=0;number(value(0xea44a8u),0x5e03fcu,70,98,0xf56f9cu);
    label(5,U(0xe20778u)+70u,98,0xf56f9cu);
    U(0xe20778u)=0;
    number(value(0xea44b4u)+value(0xea44b8u),0x5f89d8u,365,98,0xf56f9cu);
    label(23,U(0xe20778u)+365u,98,0xf56f9cu);
    label(6,270,125,0xf56f8cu);label(7,400,125,0xf56f8cu);
    for(row=0;row<sizeof(rows)/sizeof(rows[0]);++row){
        if(row==4){line(280,350,198);line(420,490,198);}
        if(row==13){line(280,350,345);line(420,490,345);}
        if(row==8)U(0xe20778u)=0;
        label(rows[row].item,80,rows[row].dy,0xf56f8cu);
        if(row==8)number(rate,0x5e03fcu,U(0xe20778u)+80u,272,0xf56f8cu);
        number(value(rows[row].left),0x57c048u,290,rows[row].dy,0xf56f8cu);
        number(value(rows[row].right),0x57c048u,430,rows[row].dy,0xf56f8cu);
    }
}
