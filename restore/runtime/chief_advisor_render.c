#include "chief_advisor_render.h"
#include "menu_building_overlay.h"
#include "town_menu.h"
#include "map_commit_population.h"
#include "flood_simulation.h"
#include "sprite_render.h"
extern void FUN_004cd680(int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d65b0(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
#define BAD U(0xf56fa4u)
#define NORMAL U(0xf56f9cu)
#define GOOD U(0xf56f94u)
static uint32_t player_offset(void){return U(0xeaac28u)*0x49d8u;}
static uint32_t value(uint32_t field){return U(field+player_offset());}
static const int16_t *point(uint32_t row){return (const int16_t *)(uintptr_t)(0x5c5878u+row*28u);}
static void text(uint32_t row,uint32_t item,uint32_t font,uint32_t dx)
{FUN_004d6200(0x3d,item,point(row),font,0,I(dx),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void heading(uint32_t row)
{text(row,row+1u,NORMAL,0);U(0xe20778u)=0;}
static void number(uint32_t row,uint32_t amount,uint32_t suffix,uint32_t font)
{FUN_004cd290(amount,0x40,(const char *)(uintptr_t)suffix,point(row),font,0,I(U(0xe20778u)+140u),0);}
static RestoreTownRecord *next(RestoreTownRecord *record)
{return restore_town_trade_next(RESTORE_TOWNS,(const int16_t *)(const void *)record);}

/* Complete52C570/1190 native instructions. Each block owns the same native
   cached values while presentation/query calls retain their live reloads.
   Signed divisions preserve wrapped32-bit products on defined game states. */
void FUN_0052c570(void)
{
    uint32_t phase=5u-U(0xf53f48u),offset,font,item,n,amount,denominator;
    int32_t level,other,minimum,count,high,ratio;RestoreTownRecord *record;
    uint8_t waiting=0,travel=0,arrived=0;
    U(0xf53f0cu)=U(0xe38da8u);U(0xf53f08u)=U(0xe38da4u);
    U(0xf53f04u)=40;U(0xf53f00u)=24;
    if(I(phase)>=5)return;
    if(I(phase)>0){
        FUN_004cd680(I(U(0xf53f0cu)+phase*16u),I(U(0xf53f08u)+phase*8u),I((40u-phase*2u)*16u),I((24u-phase*2u)*16u),0);
        return;
    }
    FUN_00536820(I(U(0xe38da8u)),I(U(0xe38da4u)),40,24);
    (void)FUN_0053e590(I(U(0xf5681cu)+11u),I(U(0xf53f0cu)+10u),I(U(0xf53f08u)+10u));
    FUN_004d62f0(0x3d,0,I(U(0xf53f0cu)+60u),I(U(0xf53f08u)+12u),U(0xf56fb4u),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
    FUN_004d62f0(0x3d,218,I(U(0xf53f0cu)+60u),I(U(0xf53f08u)+350u),U(0xf56f8cu),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
    FUN_004d65b0(I(U(0xf53f0cu)+32u),I(U(0xf53f08u)+60u),36,17,I(U(0xe38da8u)),I(U(0xe38da4u)));
    for(n=0;n<240u;n+=20u)(void)FUN_0053e590(I(U(0xf5690cu)),I(U(0xf53f0cu)+40u),I(U(0xf53f08u)+n+67u));

    heading(0);level=I(value(0xea1880u));
    if(level<=0){
        text(0,20,BAD,140);amount=value(0xea5db0u);
        if(amount)text(0,amount+32u,BAD,U(0xe20778u)+140u);
    }else if(level>=100)text(0,31,GOOD,140);
    else{
        font=level<30?BAD:level<50?NORMAL:GOOD;
        text(0,(uint32_t)(level/10)+21u,font,140);
        if(I(value(0xea1880u))<50){amount=value(0xea5db0u);if(amount)text(0,amount+31u,font,U(0xe20778u)+140u);}
    }

    heading(1);offset=player_offset();font=GOOD;
    level=I(U(0xea3fb0u+offset));
    if(!level){
        if(I(U(0xea5eb4u+offset)+U(0xea5ea4u+offset))>3){item=43;font=BAD;}
        else if(I(U(0xea3facu+offset))>=5)item=44;
        else if(!U(0xea3f98u+offset)&&I(U(0xea18a8u+offset))>0){
            if(I(U(0xea3f9cu+offset))>=80)item=44;
            else{item=U(0xea5c80u+offset)+46u;font=BAD;}
        }else{item=45;font=BAD;}
    }else if(level==1){
        font=BAD;item=!U(0xea3f98u+offset)&&I(U(0xea18a8u+offset))>0?U(0xea5c80u+offset)+53u:52u;
    }else item=59;
    text(1,item,font,140);

    heading(2);offset=player_offset();level=I(U(0xea446cu+offset));font=GOOD;item=0;
    if(level>17){item=76;font=BAD;}
    else if(level>10){item=77;font=NORMAL;}
    else if(level>5)item=78;
    else if(level>0)item=79;
    else{
        other=I(U(0xea4474u+offset));
        if(other>20){item=80;font=BAD;}
        else if(other>10){item=81;font=NORMAL;}
        else if(other>5)item=82;
        else if(other>0)item=83;
    }
    if(level<=5&&I(U(0xea4474u+offset))<=5)text(2,84,GOOD,140);
    else{
        text(2,item,font,140);
        if(level>0)number(2,value(0xea446cu),0x5e03fcu,font);
        else if(I(value(0xea4474u))>0){
            number(2,value(0xea4474u),0x57c048u,font);
            FUN_004d6200(8,13,point(2),font,0,I(U(0xe20778u)+140u),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
        }
    }

    heading(3);offset=player_offset();
    if(U(0x784bbcu))text(3,94,GOOD,140);
    else{
        count=I(U(0xea4388u+offset));
        if(count<=0)text(3,95,BAD,140);
        else{
            ratio=I(U(0xea41dcu+offset)*100u)/I(U(0xea5e1cu+offset));
            if(ratio<50)text(3,96,NORMAL,140);
            else if(count<1)text(3,97,GOOD,140);
            else{
                text(3,98,GOOD,140);number(3,value(0xea4388u),0x57c048u,GOOD);
                FUN_004d6200(8,5,point(3),GOOD,0,I(U(0xe20778u)+140u),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
            }
        }
    }

    /* Native caches the green font before this heading and food query. */
    font=GOOD;heading(4);
    if(value(0xea1894u)){
        amount=(uint32_t)FUN_00447530();ratio=I(amount*100u)/I(value(0xea1894u));
        if(ratio>150)item=13;
        else if(ratio>110)item=14;
        else if(ratio>95)item=15;
        else if(ratio>85)item=16;
        else if(ratio>70){item=17;font=NORMAL;}
        else{item=18;font=BAD;}
    }else{item=18;font=BAD;}
    text(4,item,font,U(0xe20778u)+140u);

    heading(5);level=I(value(0xea1888u));font=level<20?BAD:level<30?NORMAL:GOOD;
    text(5,(uint32_t)(level/10)+103u,font,140);

    heading(6);offset=player_offset();font=GOOD;minimum=100;count=0;high=0;
    for(n=0;n<5u;++n){level=(int8_t)B(0xea5c00u+offset+n);if(level<30)++count;if(level<minimum)minimum=level;}
    for(n=0;n<5u;++n)if((int8_t)B(0xea5c00u+offset+n)>70)++high;
    if(count){
        if(count>3)item=minimum<20?126u:125u;
        else if(count==3)item=minimum<20?128u:127u;
        else if(count==2)item=minimum<20?130u:129u;
        else item=minimum<20?132u:131u;
        if(minimum<20)font=NORMAL;
    }else item=high==0?133u:high==1?134u:high==2?135u:high==3?136u:137u;
    text(6,item,font,140);

    heading(7);offset=player_offset();font=GOOD;amount=0xffffffffu;
    denominator=U(0xea44b8u+offset)+U(0xea44b4u+offset);
    if(denominator&&I(U(0xea44b0u+offset)+U(0xea44acu+offset))/I(denominator)<1){item=151;font=NORMAL;}
    else{
        level=I(U(0xea187cu+offset));other=I(U(0xea452cu+offset));
        if(level>other){item=152;amount=(uint32_t)level-(uint32_t)other;}
        else if(level<other){item=154;font=NORMAL;amount=(uint32_t)other-(uint32_t)level;}
        else item=153;
    }
    text(7,item,font,140);if(amount!=0xffffffffu)number(7,amount,0x5f89ccu,font);

    heading(8);offset=player_offset();font=GOOD;level=I(U(0xea5fd0u+offset));amount=0xffffffffu;
    if(level>=10){item=159;font=BAD;amount=U(0xea5fd4u+offset);}
    else if(!level)item=163;
    else{amount=U(0xea5fd4u+offset);if(level<2)item=162;else if(level<5)item=161;else{item=160;font=NORMAL;}}
    text(8,item,font,140);
    if(amount!=0xffffffffu){number(8,amount,0x57c048u,font);text(8,164,font,U(0xe20778u)+140u);}

    for(record=next(0);record;record=next(record))if(W((uintptr_t)record+4u)==37u){
        if((uint8_t)FUN_0044d4c0(record))waiting=1;
        level=(int16_t)W((uintptr_t)record+48u);
        if(level==5)travel=1;
        if(level==6||level==7)arrived=1;
    }
    heading(9);offset=player_offset();font=GOOD;
    if(U(0xea5eb4u+offset)){item=174;font=BAD;}
    else if(U(0xea5ea4u+offset)){item=173;font=BAD;}
    else if(FUN_00446610()){item=172;font=BAD;}
    else if(arrived||travel)item=176;
    else if(waiting)item=175;
    else item=170u+(I(value(0xea5ee8u))>=0);
    text(9,item,font,U(0xe20778u)+140u);

    count=0;
    for(record=next(0);record;record=next(record))if((uint8_t)FUN_0044d4c0(record)){
        if(count>=5)break;
        if(W((uintptr_t)record+4u))++count;
    }
    /* Here the green font is cached before the heading as in the native. */
    font=GOOD;heading(10);item=count==0?187u:count<=2?186u:count<=4?185u:184u;
    text(10,item,font,140);

    heading(11);level=I((uint32_t)FUN_004bd590()+19u)/20;
    text(11,(uint32_t)level+192u,level==0?BAD:level==1?NORMAL:GOOD,140);
    if(level)text(12,(uint32_t)((I(U(0xd38360u))%30)/15)+U(0xd386d0u)*2u+194u,GOOD,140);
}
