#include "population_updates.h"
#include "actor_predicate.h"
#include "displaced_population.h"
#include "labor_advisor_render.h"
#include "remaining_menu_actions.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
extern int32_t FUN_004cde60(int32_t,int32_t);
static uint32_t city(void){return U(0xeaac28u)*0x49d8u;}

/* Whole458F80. The high-water count and explicit city are cached. State
   exclusions differ from the fixed state1 scans used by sentiment itself. */
int32_t FUN_00458f80(int32_t player)
{
    uint32_t off=(uint32_t)player*0x49d8u,count=U(0x78d790u),p=0x799ae0u,total=0;
    int32_t level;uint32_t people;uint8_t state;
    U(0xea3f78u+off)=0;U(0xea3f7cu+off)=0;U(0xea5eacu+off)=0;U(0xea5eb0u+off)=0;
    if(I(count)<1)return 0;
    do{
        state=B(p);
        if(state&&state!=5u&&state!=2u&&state!=6u&&B(p+5u)){
            people=(uint32_t)H(p+28u);level=H(p+18u);total+=people;
            if(level<=1)U(0xea5eacu+off)+=people;
            if(level<=3)U(0xea3f78u+off)+=people;
            if(level>=10)U(0xea5eb0u+off)+=people;
            if(level>=14)U(0xea3f7cu+off)+=people;
        }
        p+=264u;
    }while(--count);
    return I(total);
}

/* Whole4E8EC0. Thresholds are signed; wealth bands are read from the city
   selected after the percentage callback, not the city supplying its args. */
int32_t FUN_004e8ec0(void)
{
    uint32_t off=city();int32_t percentage=FUN_004cde60(I(U(0xea5eacu+off)),I(U(0xea1894u+off)));
    off=city();
    if(I(U(0xea3f7cu+off))>0){
        if(percentage>56)return 0;if(percentage>=40)return -3;if(percentage>=26)return -4;
        return percentage>=10?-5:-6;
    }
    if(I(U(0xea5eb0u+off))>0){
        if(percentage>56)return 0;if(percentage>=40)return -2;if(percentage>=26)return -3;
        return percentage>=10?-4:-5;
    }
    if(percentage>=40)return 0;if(percentage>=26)return -1;return percentage>=10?-2:-3;
}

/* Whole4E8DB0. Cache the signed wrapped city adjustment before temple
   queries; their callbacks may select another city without changing it. */
void FUN_004e8db0(void)
{
    uint32_t off=city(),adjust=(uint32_t)(I(80u-U(0xea1880u+off))/5),bonus=0,p,value;
    if(W(0xea5ce8u+off)&&FUN_004c5120(I(U(0xea5f9cu+off)))==1u){
        off=city();if((uint8_t)FUN_004c52d0(I(U(0xea5f9cu+off)),1))bonus=0xfffffffeu;
    }
    for(p=0x799ae0u;p<0x89b6d8u;p+=264u)if(B(p)==1&&B(p+5u)&&H(p+28u)){
        value=U(p+228u)+adjust+bonus;U(p+228u)=value;
        if(I(value)<0)U(p+228u)=0;
        if(I(U(p+228u))>1000)U(p+228u)=1000;
    }
}

/* Whole4E8840. Native BYTE additions wrap before signed clamping. House
   averages are unweighted; model-dependent food averages have distinct
   denominators. The stored worst cause is retained when none is negative. */
void FUN_004e8840(void)
{
    uint32_t labor=(uint32_t)FUN_004c38f0(),houses,owner,off,value,band,tax,wage=0,unemployment;
    uint32_t global_food=0,hungry_count=0,house_count=0,food_sum=0,global_sum=0,global_average=0;
    uint32_t p,level,difficulty,hunger_delta=0,food_delta,average,count,message,secondary,minimum,reason;
    uint8_t hunger,happiness;
    houses=(uint32_t)FUN_00458f80(I(U(0xeaac28u)));owner=U(0xeaac28u);off=owner*0x49d8u;
    if(I(houses)<I(U(0xea1894u+off))){
        FUN_004e9350(I(owner),I(U(0xea1894u+off)-houses));owner=U(0xeaac28u);off=owner*0x49d8u;
    }
    value=U(0xea44a8u+off);band=value?(uint32_t)(I(value+10u)/10):0;
    if(I(band)>10)band=10;
    value=U(0xea1878u+off);tax=U(0xea1384u+(value*11u+band)*4u);
    value=U(0xea4478u+off);labor=value-U(0xea447cu+off)-labor;
    if(I(labor)<0){wage=(uint32_t)(I(labor)/2);if(!wage)wage=0xffffffffu;}
    else if(I(labor)>7)wage=4;else if(I(labor)>4)wage=3;
    else if(I(labor)>1)wage=2;else if(I(labor)>0)wage=1;
    U(0xea5ea8u+off)=value;value=U(0xea446cu+off);
    unemployment=I(value)>25?0xfffffffdu:I(value)>17?0xfffffffeu:I(value)>10?0xffffffffu:I(value)<5?1u:0u;
    U(0xea5da4u+off)=value;
    if(B(0xea5efbu+off)){global_food=(uint32_t)FUN_004e8ec0();off=city();B(0xea5efbu+off)=0;}
    else B(0xea5efbu+off)=1;
    difficulty=U(0xe39080u);
    for(p=0x799ae0u;p<0x89b6d8u;p+=264u){
        if(B(p)!=1||!B(p+5u))continue;
        if(!H(p+28u)){B(p+194u)=(uint8_t)(B(0x5d199cu+difficulty*4u)+10u);continue;}
        if(I(U(0xea1894u+off))<300){
            unemployment=0;tax=0;wage=0;global_average=0;hunger_delta=0;
            B(p+194u)=(uint8_t)(B(0x5d199cu+difficulty*4u)+(I(U(0xea1894u+off))<200?10u:0u));
            U(p+228u)=0;continue;
        }
        level=(uint32_t)H(p+18u);U(p+228u)+=U(0x70ce08u+level*88u);
        B(p+194u)=(uint8_t)(B(p+194u)+(uint8_t)(unemployment+wage+tax));
        ++house_count;global_average=0;hunger_delta=0;food_delta=0;
        level=(uint32_t)H(p+18u);
        if(U(0x70cdf4u+level*88u)){
            ++hungry_count;hunger=B(p+127u);
            if(hunger>=2u){hunger_delta=2;B(p+188u)=0;food_sum+=2u;}
            else if(hunger){hunger_delta=1;B(p+188u)=0;++food_sum;}
            else{
                B(p+188u)=(uint8_t)(B(p+188u)+1u);if(B(p+188u)>3u)B(p+188u)=3;
                hunger=B(p+188u);
                hunger_delta=hunger>=3u?0xfffffffdu:hunger>=2u?0xfffffffeu:0xffffffffu;food_sum+=hunger_delta;
            }
        }else{
            food_delta=global_food;B(p+188u)=0;global_average=global_food;global_sum+=global_food;
        }
        happiness=(uint8_t)(B(p+194u)+(uint8_t)(food_delta+hunger_delta));B(p+194u)=happiness;
        if((int8_t)happiness<0)B(p+194u)=0;
        if((int8_t)B(p+194u)>100)B(p+194u)=100;
        difficulty=U(0xe39080u);
    }
    if(hungry_count)hunger_delta=(uint32_t)(I(food_sum)/I(hungry_count));
    if(house_count)global_average=(uint32_t)(I(global_sum)/I(house_count));
    average=0;count=0;
    for(p=0x799ae0u;p<0x89b6d8u;p+=264u)if(B(p)==1&&B(p+5u)&&H(p+28u)){
        average+=(uint32_t)(int32_t)(int8_t)B(p+194u);++count;
    }
    if(count){
        U(0xea1880u+off)=(uint32_t)(I(average)/I(count));
        if(W(0xea5ce8u+off)&&FUN_004c5120(I(U(0xea5f9cu+off)))==4u){
            value=city();
            if((uint8_t)FUN_004c52d0(I(U(0xea5f9cu+value)),2)){
                value=city();average=U(0xea1880u+value)+10u;
                U(0xea1880u+value)=I(average)>=100?100u:average;
            }
        }
    }else U(0xea1880u+off)=60;
    owner=U(0xeaac28u);off=owner*0x49d8u;minimum=0;
    if(I(hunger_delta)<0){minimum=hunger_delta;U(0xea5db0u+off)=1;}
    if(I(unemployment)<I(minimum)){minimum=unemployment;U(0xea5db0u+off)=2;}
    if(I(tax)<I(minimum)){minimum=tax;U(0xea5db0u+off)=3;}
    if(I(wage)<I(minimum)){minimum=wage;U(0xea5db0u+off)=4;}
    if(I(global_average)<I(minimum))U(0xea5db0u+off)=5;
    if((int32_t)(int8_t)B(0xe38ea8u)>=2||U(0xe38ebcu)){
        average=U(0xea1880u+off);
        if(I(average)/10!=I(U(0xea5da8u+off))/10&&I(U(0xea1894u+off))>300){
            secondary=0;reason=U(0xea5db0u+off);
            if(I(average)<=0){message=0x69;if(reason)secondary=reason+0x74u;}
            else if(I(average)>=100)message=0x74;
            else{message=(uint32_t)(I(average)/10)+0x6au;if(I(average)<50&&reason)secondary=reason+0x74u;}
            if(message){FUN_004e2970(I(message));owner=U(0xeaac28u);}
            if(secondary){FUN_004e2970(I(secondary));owner=U(0xeaac28u);}
        }
    }
    off=owner*0x49d8u;U(0xea5da8u+off)=U(0xea1880u+off);
}
