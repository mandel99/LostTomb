#include "menu_training_render.h"
#include "dropdown_menu.h"
#include "training_queries.h"
#include "resource_availability.h"
#include "menu_list_scroll.h"
#include "sprite_render.h"
extern void FUN_004256a0(int32_t,int32_t,const RestoreSquareButton *,int32_t);
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d63d0(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d6860(int32_t,uint32_t,const char *,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004cce90(int32_t,uint32_t,const char *,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_004ccc30(uint32_t,uint32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
#define EMPTY ((const char *)(uintptr_t)0x57c048u)
#define NORMAL U(0xf56f9cu)
static uint32_t player_offset(void){return U(0xeaac28u)*0x49d8u;}
static int32_t x(uint32_t dx){return I(U(0xf53f0cu)+dx);}
static int32_t y(uint32_t dy){return I(U(0xf53f08u)+dy);}
static void label(uint32_t group,uint32_t item,uint32_t dx,uint32_t dy,uint32_t font)
{FUN_004d62f0(group,item,x(dx),y(dy),font,0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void centered(uint32_t item,uint32_t dx)
{FUN_004d6250(0x36,item,x(dx),y(402),200,U(0xf56f8cu),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void number(uint32_t amount,uint32_t dx,uint32_t dy)
{FUN_004d6860(I(amount),0x40,EMPTY,x(dx),y(dy),NORMAL,0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void aligned(uint32_t amount,uint32_t dx,uint32_t dy)
{FUN_004cce90(I(amount),0x40,EMPTY,x(dx),y(dy),100,NORMAL,0);}
static int denominated(uint32_t resource)
{return resource==24u||resource==25u||resource==26u||resource==30u||resource==35u;}
static int divided(uint32_t resource)
{return denominated(resource)||resource==10u||resource==28u||resource==36u;}

/* Whole559E10. Availability is rebuilt even during the opening stages. */
void FUN_00559e10(void)
{
    uint32_t phase;
    FUN_005596d0();phase=5u-U(0xf53f48u);
    U(0xf53f0cu)=U(0xe38da8u);U(0xf53f08u)=U(0xe38da4u);
    U(0xf53f04u)=40;U(0xf53f00u)=27;
    if(I(phase)>=5)return;
    if(I(phase)>0){FUN_004cd680(x(phase*16u),y(phase*8u),I((40u-phase*2u)*16u),I((27u-phase*2u)*16u),0);return;}
    FUN_00536820(I(U(0xe38da8u)),I(U(0xe38da4u)),40,27);
    (void)FUN_0053e590(I(U(0xf5681cu)+4u),x(10),y(10));
    label(0x36,0,60,12,U(0xf56fb4u));
    if(!B(0xea623cu+player_offset()))label(0x36,1,100,38,U(0xf56f8cu));
    else{label(0x36,22,175,38,U(0xf56f8cu));label(0x36,23,275,38,U(0xf56f8cu));}
}

/* Whole559FC0. The caller owns16 counts, which accumulate across calls.
   Native category lookup happens after eligibility returns and can see its
   mutations; AL alone decides eligibility. No inferred category map is used. */
int32_t FUN_00559fc0(int32_t counts[16])
{
    uint32_t record,p,mode,kind,address,i,total=0;
    for(record=1,p=0x77de6fu;p<0x77f747u;++record,p+=106u){
        if(!B(p-5u)||!B(p))continue;
        mode=W(0xea40bcu+player_offset());if(mode!=1u&&mode!=3u)continue;
        if(!B(p+56u)||!(uint8_t)FUN_00447790(I(record),19))continue;
        kind=U(0x5d2a3cu+(uint32_t)B(p-2u)*8u);
        address=(uint32_t)(uintptr_t)counts+kind*4u-152u;
        U(address)+=1u;
    }
    for(i=0;i<16u;++i)if(counts[i])++total;
    return I(total);
}

/* Whole55A060 and40C760. The native arrow uses count-13; the list's later
   normalization uses count-14. Keep those distinct transient publications. */
void FUN_0055a060(void)
{
    uint32_t count=U(0xf53e6cu),first,percentage,offset;
    if(I(count-15u)<=0)return;
    first=U(0xea60a4u+player_offset());
    if(I(first)<=1)percentage=0;
    else if(I(first+14u)>I(count))percentage=100;
    else percentage=(uint32_t)FUN_004cde60(I(first-1u),I(count-15u));
    offset=(uint32_t)FUN_004cde40(256,I(percentage));
    (void)FUN_0053e590(I(U(0xf56494u)+39u),x(597),y(78u+offset));
}
void FUN_0040c760(void)
{
    uint32_t p=0xea60a4u+player_offset(),next,count;
    if(!U(0xd3b198u)){next=U(p)-1u;U(p)=next;if(I(next)<=0)U(p)=1;}
    else{next=U(p)+1u;U(p)=next;count=U(0xf53e6cu);if(I(next+13u)>I(count))U(p)=count-13u;}
    B(0xe38dcau)=1;
}

static void scroll_button(uint32_t button,uint32_t dx,uint32_t dy)
{
    RestoreSquareButton *p=(RestoreSquareButton *)(uintptr_t)button;
    FUN_00425e80(x(dx),y(dy),p,1,10);
    (void)FUN_00425c40(x(dx),y(dy),p,1);
    FUN_004256a0(x(dx),y(dy),p,1);
}
static void luxury(uint32_t mode,uint32_t row,uint32_t dy,int32_t counts[16])
{
    uint32_t categories,i;
    U(0xe20778u)=0;categories=(uint32_t)FUN_00559fc0(counts);
    label(0x36,mode==1u?5u:37u,324,dy+61u,NORMAL);
    if(mode==1u)number((uint32_t)(int32_t)H(0xea4104u+player_offset())*100u,U(0xe20778u)+324u,dy+61u);
    if(categories==1u){
        for(i=0;i<16u;++i)if(counts[i]){label(0x17,i+39u,U(0xe20778u)+324u,dy+61u,NORMAL);return;}
    }else if(categories){
        W(0x603326u)=(uint16_t)(row*22u+76u);W(0x603322u)=(uint16_t)(row*22u+56u);
        label(0x36,45,U(0xe20778u)+324u,dy+61u,NORMAL);
    }
}
static void status(uint32_t resource,uint32_t row,uint32_t dy,int32_t counts[16])
{
    uint32_t font=NORMAL,mode=W(0xea4096u+player_offset()+resource*2u),item,amount;
    if(mode==0u){
        if(FUN_00443950(I(resource))&&FUN_00443b20(I(resource)))item=33;
        else if(FUN_00443950(I(resource)))item=31;
        else if(FUN_00443b20(I(resource)))item=32;
        else{
            if(FUN_00443a20(I(resource))&&FUN_004439c0(I(resource)))item=36;
            else if(FUN_004439c0(I(resource)))item=34;
            else if(FUN_00443a20(I(resource)))item=35;
            else return;
            font=U(0xf56f94u);
        }
        label(0x36,item,324,dy+61u,font);return;
    }
    if(mode==1u||mode==2u){
        U(0xe20778u)=0;
        if(mode==1u&&resource==19u){luxury(mode,row,dy,counts);return;}
        label(0x36,mode==1u?5u:6u,324,dy+61u,font);
        amount=(uint32_t)(int32_t)H(0xea40deu+player_offset()+resource*2u)*100u;
        if(divided(resource))amount=(uint32_t)(I(amount)/100);
        if(mode==1u&&denominated(resource))FUN_004ccc30(8,56,I(amount),x(U(0xe20778u)+324u),y(dy+61u),NORMAL,0);
        else number(amount,U(0xe20778u)+324u,dy+61u);
        return;
    }
    if(mode==3u&&resource==19u){luxury(mode,row,dy,counts);return;}
    label(0x36,mode==3u?37u:38u,324,dy+61u,font);
}

/* Whole55A0E0. Own the16-bin scratch once, retain the native do/while's
   initial row and at most15 rows, and keep the pre-input list cursor cached. */
void FUN_0055a0e0(void)
{
    int32_t counts[16]={0};uint32_t p=0xea60a4u+player_offset(),first=U(p),count=U(0xf53e6cu);
    uint32_t cursor,row=0,dy=0,resource,amount,font,item;
    if(I(count-first+1u)<15)U(p)=count-14u;
    if(I(U(p))<1)U(p)=1;
    first=U(p);
    if(I(U(0xf53f48u))<=5)return;
    FUN_004d65b0(x(16),y(52),36,21,I(U(0xe38da8u)),I(U(0xe38da4u)));
    if(I(U(0xf53e6cu))>=15){
        scroll_button(0x5c5b90u,573,52);scroll_button(0x5c5b78u,597,364);
        FUN_004d65b0(x(593),y(76),2,18,I(U(0xe38da8u)),I(U(0xe38da4u)));
    }
    cursor=0xf53e6cu+first*4u;
    W(0x603322u)=0;W(0x603326u)=0;
    do{
        resource=U(cursor);
        (void)FUN_0053e590(I(U(0xf571fcu)+resource),x(22),y(dy+57u));
        if(row==U(0x7056c8u)-4u)FUN_004d63d0(0,x(20),y(dy+54u),570,24,1,I(U(0xe38da8u)),I(U(0xe38da4u)));
        font=W(0xea4126u+player_offset()+resource*2u)==1u?U(0xf56fa4u):NORMAL;
        label(0x17,resource,44,dy+61u,font);
        if(!B(0xea623cu+player_offset())){
            amount=U(0xea6160u+player_offset()+resource*4u);
            if(divided(resource))amount=(uint32_t)(I(amount)/100);
            aligned(amount,140,dy+61u);
            if(U(0xea42f8u+player_offset()+resource*4u))label(0x36,3,232,dy+61u,NORMAL);
            status(resource,row,dy,counts);
        }else{
            aligned(U(0x5d2c48u+resource*8u),175,dy+61u);
            aligned(U(0x5d2c4cu+resource*8u),275,dy+61u);
            item=denominated(resource)?49u:resource==10u?47u:resource==28u?48u:50u;
            label(0x36,item,425,dy+61u,NORMAL);
        }
        ++first;cursor+=4u;
        if(I(first)>I(U(0xf53e6cu)))break;
        dy+=22u;++row;
    }while(dy<330u);
    FUN_004d63d0(0,x(398),y(396),200,24,U(0x7056c8u)==1u,I(U(0xe38da8u)),I(U(0xe38da4u)));
    centered(B(0xea623cu+player_offset())?46u:2u,400);
    FUN_004d63d0(0,x(98),y(396),200,24,U(0x7056c8u)==2u,I(U(0xe38da8u)),I(U(0xe38da4u)));
    centered(30,100);FUN_0055a060();
}
