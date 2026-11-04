#include "message_category.h"
#include "town_menu.h"
#include "town_record_queries.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
static uint32_t city_offset(void){return U(0xeaac28u)*0x49d8u;}
static int32_t signed_percent(uint32_t amount,uint32_t total)
{
    int32_t numerator=(int32_t)(amount*100u),denominator=(int32_t)total;
    /* Preserve native IDIV's invalid states rather than fabricating a value. */
    if(!denominator||(numerator==INT32_MIN&&denominator==-1))abort();
    return numerator/denominator;
}
uint32_t restore_actor_building_work_state(uint32_t actor)
{
    uint32_t building=(uint32_t)(int32_t)(int16_t)W(0xbcd656u+actor*388u)*264u;
    uint16_t kind;uint32_t slot;
    if(B(0x7999d8u+building)!=1u)return 1;
    kind=W(0x7999e8u+building);
    if(kind!=75u&&kind!=192u)return 1;
    for(slot=0;slot<3u;++slot){
        int32_t linked=(int16_t)W(0x799a86u+building+slot*2u);uint32_t record;uint8_t state;
        if(!linked)continue;
        record=(uint32_t)linked*388u;
        if(B(0xbcd606u+record)!=1u)continue;
        state=B(0xbcd630u+record);
        if(state==9u||state==11u||state==15u||state==14u)return 1;
        if(state==10u||state==12u||state==13u||state==16u)return 2;
    }
    return 0;
}
uint32_t restore_message_build_category_weights(volatile uint8_t *weights,volatile uint8_t *all_defaults)
{
    RestoreTownRecord *record;uint32_t blocked=0,current=0,index,weight=0,sum=0;
    *(volatile uint32_t *)(void *)weights=0;
    *(volatile uint32_t *)(void *)(weights+4)=0;
    record=restore_town_next(RESTORE_TOWNS,0,0);
    while(record){
        uint32_t flags=U((uint32_t)(uintptr_t)record+0x28u);
        if(flags&0x10u){blocked=(U((uint32_t)(uintptr_t)record+0x28u)>>2)&1u;break;}
        record=restore_town_next(RESTORE_TOWNS,(const int16_t *)record,0);
    }
    record=restore_town_next(RESTORE_TOWNS,0,0);
    while(record){
        uint32_t address=(uint32_t)(uintptr_t)record;
        if((uint8_t)restore_event_within_window_result(address,address)){current=1;break;}
        record=restore_town_next(RESTORE_TOWNS,(const int16_t *)record,0);
    }
    for(index=0;index<8u;++index){
        uint32_t city,clear=0;int32_t value;
        switch(index){
        case 0:
            value=(int32_t)U(0xea1888u+city_offset());
            weight=value<20?8u:value<30?6u:value<40?4u:1u;clear=weight!=1u;break;
        case 1:
            value=(int32_t)U(0xea4388u+city_offset());
            weight=value<1?6u:value<2?4u:1u;clear=weight!=1u;break;
        case 2:
            if(!current)weight=1;
            else if(!U(0xbcb9b4u)&&!B(0xea5ed8u+city_offset())){weight=8;clear=1;}
            /* Otherwise EBX retains the preceding category's weight. */
            break;
        case 3:{
            uint32_t demand=0,labor=0,i;city=city_offset();
            for(i=0;i<10u;++i)demand+=U(0xea43a0u+city+i*20u);
            for(i=0;i<10u;++i)labor+=U(0xea439cu+city+i*20u);
            if(!labor)weight=1;
            else{value=(int32_t)(100u-(uint32_t)signed_percent(demand,labor));weight=value>20?8u:value>10?6u:value>5?4u:1u;}
            clear=weight!=1u;break;}
        case 4:{
            uint32_t i;int32_t minimum=100;city=city_offset();
            for(i=0;i<5u;++i){value=(int8_t)B(0xea5c0au+city+i);if(value<minimum)minimum=value;}
            weight=minimum<=20?6u:minimum<=30?4u:1u;clear=weight!=1u;break;}
        case 5:
            if(blocked)weight=0;
            else{value=(int32_t)U(0xea5b48u+city_offset());weight=value<=0?8u:value<20?6u:value<40?4u:1u;clear=weight!=1u;}
            break;
        case 6:{
            uint32_t total;city=city_offset();total=U(0xea1894u+city);
            if(!total)weight=1;
            else{value=signed_percent(U(0xea4468u+city),total);weight=value>17?8u:value>10?6u:value>5?4u:1u;}
            clear=weight!=1u;break;}
        case 7:
            value=(int32_t)U(0xea5bf4u+city_offset());weight=value<=0?6u:value<20?4u:1u;clear=weight!=1u;break;
        }
        if(clear)*all_defaults=0;
        sum+=weight;weights[index]=(uint8_t)weight;
    }
    {uint8_t a=weights[3];
     if(a==1u&&weights[6]>=1u){weights[3]=0;return sum-1u;}
     if(a>1u&&weights[6]==1u){weights[6]=0;return sum-1u;}}
    return sum;
}
uint32_t restore_message_choose_weighted_category(void)
{
    uint8_t all_defaults=1;uint32_t total,percent_sum=0,index,selector,cumulative=0;
    total=FUN_0053d3a0((volatile uint8_t *)(uintptr_t)0xf55ba4u,&all_defaults);
    for(index=0;index<8u;++index)percent_sum+=(uint32_t)signed_percent(B(0xf55ba4u+index),total);
    selector=U(0xd3b374u)&percent_sum;
    if(all_defaults)return 9;
    for(index=0;index<8u;++index){
        cumulative+=(uint32_t)signed_percent(B(0xf55ba4u+index),total);
        if((int32_t)selector<=(int32_t)cumulative)break;
    }
    if(index==8u)index=7;
    return B(0xf55ba4u+index)==1u?8u:index;
}
