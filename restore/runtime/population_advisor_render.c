#include "population_advisor_render.h"
#include "map_commit_population.h"
#include "sprite_render.h"
extern void FUN_004cd680(int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d63d0(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d65b0(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d6860(int32_t,uint32_t,const char *,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004ccc30(uint32_t,uint32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_004d6160(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004cce90(int32_t,uint32_t,const char *,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_004cccb0(uint32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_004cd510(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_004cd9d0(int32_t,int32_t,int32_t,int32_t,uint32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
#define EMPTY ((const char *)(uintptr_t)0x57c048u)
#define NORMAL U(0xf56f9cu)
#define GOOD U(0xf56f94u)
static uint32_t player_offset(void){return U(0xeaac28u)*0x49d8u;}
static uint32_t value(uint32_t field){return U(field+player_offset());}
static int32_t x(uint32_t dx){return I(U(0xf53f0cu)+dx);}
static int32_t y(uint32_t dy){return I(U(0xf53f08u)+dy);}
static void label(uint32_t group,uint32_t item,uint32_t dx,uint32_t dy,uint32_t font)
{FUN_004d62f0(group,item,x(dx),y(dy),font,0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void number(uint32_t amount,uint32_t dx,uint32_t dy,uint32_t font)
{FUN_004d6860(I(amount),0x40,EMPTY,x(dx),y(dy),font,0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void box(uint32_t dy,uint32_t selected)
{FUN_004d63d0(0,x(501),y(dy),106,57,I(selected),I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void centered(uint32_t item,uint32_t dx,uint32_t dy,uint32_t width)
{FUN_004d6250(0x37,item,x(dx),y(dy),width,U(0xf56f8cu),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void migration_reasons(uint32_t with_cursor)
{
    static const uint32_t items[6]={21,22,23,24,32,33};
    uint32_t i;
    /* Independent native checks: drawing a reason can change player/reason. */
    for(i=0;i<6u;++i)if(value(0xea5c80u)==i)
        label(0x37,items[i],75u+((i||with_cursor)?U(0xe20778u):0u),396,NORMAL);
}

/* Complete525CB0: six graph layouts and all food/storage/migration reports. */
void FUN_00525cb0(void)
{
    static const uint8_t layouts[6][3]={{0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}};
    static const uint8_t titles[6][3]={{6,4,5},{6,5,4},{7,3,5},{7,5,3},{8,3,4},{8,4,3}};
    static void (*const graphs[3])(int32_t,int32_t,int32_t)={FUN_00527360,FUN_00527710,FUN_005278e0};
    static const uint32_t markers[5]={310,344,362,380,398};
    uint32_t phase=5u-U(0xf53f48u),selection,i,amount,font,item,offset,state;
    int32_t ratio,level;
    U(0xf53f0cu)=U(0xe38da8u);U(0xf53f08u)=U(0xe38da4u);
    U(0xf53f04u)=40;U(0xf53f00u)=27;
    if(I(phase)>=5)return;
    if(I(phase)>0){
        FUN_004cd680(x(phase*16u),y(phase*8u),I((40u-phase*2u)*16u),I((27u-phase*2u)*16u),0);return;
    }
    FUN_00536820(I(U(0xe38da8u)),I(U(0xe38da4u)),40,27);
    (void)FUN_0053e590(I(U(0xf5681cu)+5u),x(10),y(10));
    selection=U(0x605a40u);
    if(I(selection)>=0)label(0x37,I(selection)>=4?2u:I(selection)>=2?1u:0u,60,12,U(0xf56fb4u));
    (void)FUN_0053e590(I(U(0xf55c5cu)+14u),x(56),y(60));
    box(60,0);box(160,0);
    selection=U(0x605a40u);
    if(selection<6u){
        centered(titles[selection][0],60,280,400);
        centered(titles[selection][1],504,120,100);
        centered(titles[selection][2],504,220,100);
        graphs[layouts[selection][0]](1,x(64),y(64));
        graphs[layouts[selection][1]](0,x(505),y(63));
        graphs[layouts[selection][2]](0,x(505),y(163));
    }
    FUN_004d65b0(x(48),y(305),34,7,I(U(0xe38da8u)),I(U(0xe38da4u)));
    for(i=0;i<5u;++i)(void)FUN_0053e590(I(U(0xf5690cu)),x(56),y(markers[i]));
    ratio=0;
    if(value(0xea1894u)){
        amount=(uint32_t)FUN_00447530()*100u;
        ratio=I(amount)/I(value(0xea1894u));
    }
    amount=(uint32_t)FUN_00447530();font=GOOD;
    if(ratio>150)item=51;
    else if(ratio>110)item=48;
    else if(ratio>95)item=45;
    else if(ratio>85)item=42;
    else if(ratio>70){item=39;font=NORMAL;}
    else{item=36;font=U(0xf56fa4u);}
    level=I(value(0xea3f9cu));if(level<0)++item;else if(level>0)item+=2;
    U(0xe20778u)=0;label(0x37,34,75,308,GOOD);
    number(amount,U(0xe20778u)+75u,308,GOOD);
    label(0x37,35,U(0xe20778u)+75u,308,GOOD);
    label(0x37,item,75,324,font);
    if(U(0x784bbcu))label(0x37,11,75,342,NORMAL);
    else{
        U(0xe20778u)=0;
        FUN_004ccc30(8,6,I(value(0xea438cu)),x(75),y(342),NORMAL,0);
        offset=player_offset();
        if(I(U(0xea4388u+offset))<=0){
            level=I(U(0xea41dcu+offset));
            item=level>I(U(0xea5e1cu+offset))/2?13u:level<=0?14u:15u;
            label(0x37,item,U(0xe20778u)+75u,342,NORMAL);
        }else{
            label(0x37,12,U(0xe20778u)+75u,342,NORMAL);
            FUN_004ccc30(8,4,I(value(0xea4388u)),x(U(0xe20778u)+75u),y(342),NORMAL,0);
        }
    }
    amount=value(0xea18a8u);U(0xe20778u)=0;
    label(0x37,16,75,360,NORMAL);number(amount,U(0xe20778u)+75u,360,NORMAL);
    label(0x37,17,U(0xe20778u)+75u,360,NORMAL);
    offset=player_offset();state=U(0xea3fb0u+offset);
    if(!state){
        U(0xe20778u)=0;
        if(I(U(0xea5eb4u+offset)+U(0xea5ea4u+offset))>3){
            label(0x37,26,75,378,NORMAL);U(0xe20778u)=0;label(0x3d,43,75,396,NORMAL);return;
        }
        if(I(U(0xea3facu+offset))>=5){
            label(0x37,25,75,378,NORMAL);U(0xe20778u)=0;
            number(value(0xea3facu),75,396,NORMAL);label(0x37,18,U(0xe20778u)+75u,396,NORMAL);return;
        }
        if(U(0xea3f98u+offset)||I(U(0xea18a8u+offset))<=0){
            label(0x37,25,75,378,NORMAL);U(0xe20778u)=0;label(0x37,20,75,396,NORMAL);return;
        }
        if(I(U(0xea3f9cu+offset))>=80){
            label(0x37,25,75,378,NORMAL);U(0xe20778u)=0;
            number(value(0xea3facu),75,396,NORMAL);
            label(0x37,value(0xea3facu)==1u?19u:18u,U(0xe20778u)+75u,396,NORMAL);return;
        }
        label(0x37,26,75,378,NORMAL);U(0xe20778u)=0;migration_reasons(0);
    }else if(state==1u){label(0x37,26,75,378,NORMAL);migration_reasons(1);}
    else FUN_004d6160(0x37,state==2u?30u:31u,x(75),y(378),496,NORMAL,0,I(U(0xe38da8u)),I(U(0xe38da4u)));
}

void FUN_005271f0(void)
{
    uint32_t hover=U(0x7056c8u);
    if(hover==U(0x5f825cu)||hover>2u)return;
    U(0x5f825cu)=hover;box(60,hover==1u);box(160,hover==2u);
}

static void aligned(uint32_t amount,uint32_t px,uint32_t py,uint32_t width)
{FUN_004cce90(I(amount),0x40,EMPTY,I(px),I(py),I(width),U(0xf56f84u),0);}
static void vertical_axis(uint32_t px,uint32_t py)
{
    aligned(U(0xf53dccu),px-66u,py-3u,60);
    aligned((uint32_t)(I(U(0xf53dccu))/2),px-66u,py+96u,60);
    aligned(0,px-66u,py+196u,60);
}
static uint32_t shifted(uint32_t amount,uint32_t shift)
{return shift==UINT32_MAX?amount*2u:(uint32_t)(I(amount)>>(shift&31u));}

/* Whole527360 history graph: real ring queries, calendar and five bar widths. */
void FUN_00527360(int32_t mode,int32_t px,int32_t py)
{
    int32_t count=I(value(0xea3e30u)),maximum=0,n;
    uint32_t i,amount,step,image,xpos=(uint32_t)px,ypos=(uint32_t)py;
    count=count<=20?20:count<=40?40:count<=100?100:count<=200?200:400;
    if(!mode)count=count<=40?20:100;
    for(n=0;n<count;++n){int32_t sample=FUN_004e9760(count,n);if(sample>maximum)maximum=sample;}
    FUN_00527ad0(maximum);
    if(!mode){
        U(0xf53dc8u)+=2u;
        for(i=0;i<(uint32_t)count;++i){
            amount=(uint32_t)(FUN_004e9760(count,I(i))>>(U(0xf53dc8u)&31u));
            if(I(amount)>0){
                if(count==20)FUN_004cd9d0(I(xpos+i*5u),I(ypos-amount+50u),4,I(amount+1u),0xf800);
                else FUN_004cd510(I(xpos+i),I(ypos-amount+50u),I(xpos+i),I(ypos+50u),0xf800);
            }
        }
        return;
    }
    vertical_axis(xpos,ypos);(void)FUN_004e97c0(count);
    U(0xe20778u)=0;
    FUN_004d62f0(0x19,U(0xea180cu),I(xpos-20u),I(ypos+210u),U(0xf56f84u),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
    FUN_004cccb0(0,I(U(0xea1808u)),I(U(0xe20778u)+xpos-20u),I(ypos+210u),U(0xf56f84u),0);
    U(0xe20778u)=0;
    FUN_004d62f0(0x19,U(0xea1804u),I(xpos+380u),I(ypos+210u),U(0xf56f84u),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
    FUN_004cccb0(0,I(U(0xea1800u)),I(U(0xe20778u)+xpos+380u),I(ypos+210u),U(0xf56f84u),0);
    U(0xd3b550u)=ypos+200u;
    for(i=0;i<(uint32_t)count;++i){
        amount=(uint32_t)FUN_004e9760(count,I(i));amount=shifted(amount,U(0xf53dc8u));
        if(I(amount)>0){
            if(count==400)FUN_004cd510(I(xpos+i),I(ypos-amount+200u),I(xpos+i),I(ypos+199u),0xf800);
            else{
                step=count==20?20u:count==40?10u:count==100?4u:2u;
                image=U(0xf56904u)+(count==20?0u:count==40?1u:count==100?2u:3u);
                (void)FUN_0053e590(I(image),I(xpos+i*step),I(ypos-amount+200u));
            }
        }
    }
    U(0xd3b550u)=U(0xe38dacu);
}

/* Whole527710/5278E0 histograms. Native caches the large-graph shift until
   a positive bar is drawn, then reloads it; each mini bar reads it afresh. */
static void histogram(uint32_t social,int32_t mode,uint32_t px,uint32_t py)
{
    uint32_t i,count=social?20u:100u,base=player_offset(),shift,amount,field;
    int32_t maximum=0,sample;
    for(i=0;i<count;++i){
        sample=social?I(U(0xea3efcu+base+i*4u)):(int32_t)H(0xea3e34u+base+i*2u);
        if(sample>maximum)maximum=sample;
    }
    FUN_00527ad0(maximum);
    if(!mode){
        U(0xf53dc8u)+=2u;
        for(i=0;i<count;++i){
            field=player_offset();
            amount=social?U(0xea3efcu+field+i*4u):(uint32_t)(int32_t)H(0xea3e34u+field+i*2u);
            amount=(uint32_t)(I(amount)>>(U(0xf53dc8u)&31u));
            if(I(amount)>0){
                if(social)FUN_004cd9d0(I(px+i*5u),I(py-amount+50u),4,I(amount+1u),0xf800);
                else FUN_004cd510(I(px+i),I(py-amount+50u),I(px+i),I(py+50u),0xf800);
            }
        }
        return;
    }
    vertical_axis(px,py);
    if(social){
        FUN_004d6250(0x37,9,I(px-80u),I(py+210u),200,U(0xf56f84u),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
        FUN_004d6250(0x37,10,I(px+280u),I(py+210u),200,U(0xf56f84u),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
    }else for(i=0;i<=100u;i+=10u)aligned(i,px-22u+i*4u,py+210u,40);
    shift=U(0xf53dc8u);U(0xd3b550u)=py+200u;
    for(i=0;i<count;++i){
        field=player_offset();
        amount=social?U(0xea3efcu+field+i*4u):(uint32_t)(int32_t)H(0xea3e34u+field+i*2u);
        amount=shifted(amount,shift);
        if(I(amount)>0){
            (void)FUN_0053e590(I(U(0xf56904u)+(social?0u:2u)),I(px+i*(social?20u:4u)),I(py-amount+200u));
            shift=U(0xf53dc8u);
        }
    }
    U(0xd3b550u)=U(0xe38dacu);
}
void FUN_00527710(int32_t mode,int32_t px,int32_t py){histogram(0,mode,(uint32_t)px,(uint32_t)py);}
void FUN_005278e0(int32_t mode,int32_t px,int32_t py){histogram(1,mode,(uint32_t)px,(uint32_t)py);}
