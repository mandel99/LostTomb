#include "chief_advisor_render.h"
#include "town_menu.h"
#include "map_commit_population.h"
#include "flood_simulation.h"
#include "pixel_access.h"
#include "menu_building_overlay.h"
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d6160(uint32_t,uint32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_00425730(int32_t,int32_t,const char *,uint32_t);
static uint32_t player_offset(void){return U(0xeaac28u)*0x49d8u;}
static RestoreTownRecord *next_town(RestoreTownRecord *record)
{return restore_town_trade_next(RESTORE_TOWNS,(const int16_t *)(const void *)record);}

/* Whole538050: twelve chief-advisor advice categories. Player/table/layout
   values are DWORDs; only deity moods and action availability are BYTEs.
   Cache values inside each category and reload player/layout after queries. */
void restore_chief_advisor_render_advice(void)
{
    int32_t text=-1,action=0,level,other,ratio,low,high,minimum,n,count;
    uint32_t p,reason,denominator;RestoreTownRecord *town;
    int trade_available=0,trade_waiting=0,trade_active=0;
    switch(U(0xd3b198u)){
    case 0:
        p=player_offset();reason=U(0xea5db0u+p);
        if(reason==1u||reason==5u)action=7;else if(reason==2u||reason==4u)action=1;else if(reason==3u)action=12;
        level=I(U(0xea1880u+p));if(level<50)text=I(reason+37u);else if(level<100){text=42;action=0;}
        break;
    case 1:
        p=player_offset();text=I(U(0xea5c80u+p));
        if((uint32_t)text<=1u)action=1;else if((uint32_t)text<=3u)action=7;
        level=I(U(0xea3fb0u+p));
        if(level==1){
            if(I(U(0xea5eb4u+p)+U(0xea5ea4u+p))>=4)text=-1;
            else if(I(U(0xea3facu+p))>=5){text=68;action=0;}
            else if(!U(0xea3f98u+p)&&I(U(0xea18a8u+p))>0){
                if(I(U(0xea3f9cu+p))<80)text=I((uint32_t)text+61u);else{text=68;action=0;}
            }else{text=60;action=7;}
        }else if(!level){
            if(!U(0xea3f98u+p)&&I(U(0xea18a8u+p))>0)text=I((uint32_t)text+70u);
            else{text=69;action=7;}
        }else{text=67;action=0;}
        break;
    case 2:
        action=1;p=player_offset();level=I(U(0xea446cu+p));
        if(level>=18)text=85;else if(level>=11)text=86;else if(level>=6)text=87;else if(level>0)text=88;
        else{
            other=I(U(0xea4474u+p));
            if(other>=21)text=89;else if(other>=11)text=90;else if(other>=6)text=91;else if(other>0)text=92;
        }
        if(level<6&&I(U(0xea4474u+p))<6)text=93;
        break;
    case 3:
        p=player_offset();level=I(U(0xea4388u+p));action=7;
        if(level<1)text=99;
        else if(I(U(0xea41dcu+p)*100u)/I(U(0xea5e1cu+p))<50)text=100;
        else text=101+(level>0);
        break;
    case 4:
        if(!U(0xea1894u+player_offset()))text=212;
        else{
            level=FUN_00447530();ratio=I((uint32_t)level*100u)/I(U(0xea1894u+player_offset()));
            text=ratio>150?217:ratio>110?216:ratio>95?215:ratio>85?214:ratio>70?213:212;
        }
        action=7;break;
    case 5:action=8;text=I((uint32_t)(I(U(0xea1888u+player_offset()))/10)+114u);break;
    case 6:
        p=player_offset();low=0;high=0;minimum=100;action=11;
        for(n=0;n<5;++n){level=(int32_t)(int8_t)B(0xea5c00u+p+(uint32_t)n);if(level<40)++low;if(level<minimum)minimum=level;}
        for(n=0;n<5;++n)if((int8_t)B(0xea5c00u+p+(uint32_t)n)>60)++high;
        if(low>=4)text=138+(minimum<20);
        else if(low==3)text=140+(minimum<20);
        else if(low==2)text=142+(minimum<20);
        else if(low==1)text=144+(minimum<20);
        else if(high<3)text=146+high;
        break;
    case 7:
        p=player_offset();action=12;denominator=U(0xea44b8u+p)+U(0xea44b4u+p);
        if(denominator&&I(U(0xea44b0u+p)+U(0xea44acu+p))/I(denominator)<1)text=155;
        else{
            level=I(U(0xea187cu+p));other=I(U(0xea452cu+p));text=level>other?156:157+(level<other);
        }break;
    case 8:
        level=I(U(0xea5fd0u+player_offset()));action=12;
        text=level>=10?165:level==0?169:level<2?168:166+(level<5);break;
    case 9:
        for(town=next_town(0);town;town=next_town(town)){
            if((int16_t)W((uintptr_t)town+4u)==37){
                if((uint8_t)FUN_0044d4c0(town))trade_available=1;
                level=(int32_t)(int16_t)W((uintptr_t)town+48u);
                if(level==5)trade_waiting=1;if(level==6||level==7)trade_active=1;
            }
        }
        action=2;p=player_offset();
        if(U(0xea5eb4u+p))text=181;
        else if(U(0xea5ea4u+p))text=180;
        else if(FUN_00446610())text=179;
        else if(trade_active||trade_waiting)text=183;
        else if(trade_available)text=182;
        else text=177+(I(U(0xea5ee8u+player_offset()))>=0);
        break;
    case 10:
        count=0;
        for(town=next_town(0);town;town=next_town(town)){
            if((uint8_t)FUN_0044d4c0(town)){
                if(count>4)break;
                if((int16_t)W((uintptr_t)town+4u)!=0)++count;
            }
        }
        action=4;text=!count?191:count<3?190:188+(count<5);break;
    case 11:action=7;level=FUN_004bd590();text=I((uint32_t)(I((uint32_t)level+19u)/20)+198u);break;
    default:break;
    }
    U(0xea5fd8u+player_offset())=(uint32_t)action;
    if(text<0)return;
    FUN_004cf090(I(U(0xf53f0cu)+32u),I(U(0xf53f08u)+60u),576,272,0);
    FUN_00536820(I(U(0xf53f0cu)+16u),I(U(0xf53f08u)+144u),38,11);
    FUN_004d6160(61,(uint32_t)text,I(U(0xf53f0cu)+32u),I(U(0xf53f08u)+160u),612,U(0xf56f8cu),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
    FUN_004d6250(13,1,I(U(0xf53f0cu)+16u),I(U(0xf53f08u)+296u),608,U(0xf56f8cu),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
    if(B(0x6fdc88u+(uint32_t)action))FUN_00425730(I(U(0xf53f0cu)+32u),I(U(0xf53f08u)+272u),
        (const char *)(uintptr_t)U(0x5f78f0u+(uint32_t)action*4u),1);
}
void FUN_00538050(void){restore_chief_advisor_render_advice();}
