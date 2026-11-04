#include "actor_message.h"
#include "message_category.h"
#include "town_menu.h"
#include "town_record_queries.h"
#include "building_processing_query.h"
#include "actor_predicate.h"
#include "storage_trade.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define ACTOR_KIND(r) B(0xbcd602u+(r))
#define ACTOR_STATE(r) B(0xbcd630u+(r))
#define ACTOR_COUNTER(r) B(0xbcd674u+(r))
#define ACTOR_MESSAGE(r) B(0xbcd675u+(r))
#define ACTOR_GROUP(r) B(0xbcd729u+(r))
#define MESSAGE_RESULT U(0xeadcdcu)
static uint32_t city_offset(void){return U(0xeaac28u)*0x49d8u;}
static void cycle_counter(uint32_t record,uint32_t limit,uint8_t reset)
{
    uint8_t next=(uint8_t)(ACTOR_COUNTER(record)+1u);
    ACTOR_COUNTER(record)=next;
    if(next>=limit)ACTOR_COUNTER(record)=reset;
}
static uint32_t actor_target_distance(uint32_t record)
{
    int32_t target_x=(int16_t)W(0xbcd624u+record),x=(int16_t)W(0xbcd61cu+record);
    int32_t target_y=(int16_t)W(0xbcd626u+record),y=(int16_t)W(0xbcd61eu+record);
    uint32_t dx=(uint32_t)(x>target_x?x-target_x:target_x-x);
    uint32_t dy=(uint32_t)(y>target_y?y-target_y:target_y-y);
    U(0xd3b4fcu)=dx;U(0xd3b4f8u)=dy;
    return dx>=dy?dx:dy;
}
static uint32_t commit_message(uint32_t record,uint32_t result)
{
    ACTOR_MESSAGE(record)=(uint8_t)result;MESSAGE_RESULT=result;return result;
}
static uint32_t finish_message(uint32_t record,uint32_t result)
{
    static const uint8_t groups[8]={8,13,2,1,11,5,1,10};
    if(result==UINT32_MAX){
        switch(ACTOR_KIND(record)){
        case 5:case 7:case 10:case 15:case 16:case 17:case 18:case 27:
        case 29:case 30:case 31:case 32:case 34:case 35:case 39:case 40:
        case 87:case 88:case 89:case 105:
            result=FUN_0053c060();MESSAGE_RESULT=result;
            if(result==9u||result==8u)ACTOR_GROUP(record)=13;
            else ACTOR_GROUP(record)=groups[result];
            break;
        }
    }
    ACTOR_MESSAGE(record)=(uint8_t)result;return result;
}
static int32_t employment_percent(uint32_t city)
{
    uint32_t a=U(0xea4494u+city),b=U(0xea4490u+city);
    uint32_t c=U(0xea449cu+city),d=U(0xea4498u+city);
    int32_t numerator=(int32_t)((a+b)*100u),denominator=(int32_t)(c+d+a+b);
    if(!denominator||(numerator==INT32_MIN&&denominator==-1))abort();
    return numerator/denominator;
}
uint32_t restore_actor_update_message_result(int32_t actor,uint32_t incoming_eax)
{
    RestoreTownRecord *event;uint32_t record,current=0,value,city,kind;uint8_t state,amount;
    if(actor<=0)return incoming_eax;
    record=(uint32_t)actor*388u;
    ACTOR_MESSAGE(record)=0;MESSAGE_RESULT=UINT32_MAX;
    event=restore_town_next(RESTORE_TOWNS,0,0);
    while(event){
        uint32_t address=(uint32_t)(uintptr_t)event;
        if((uint8_t)restore_event_within_window_result(address,address)){current=1;break;}
        event=restore_town_next(RESTORE_TOWNS,(const int16_t *)event,0);
    }
    ACTOR_GROUP(record)=0;kind=ACTOR_KIND(record);
    switch(kind){
    case 1:
        cycle_counter(record,3,0);value=ACTOR_COUNTER(record);MESSAGE_RESULT=value;break;
    case 2:
        value=U(0xea5db0u+city_offset());
        if(value==1u){ACTOR_GROUP(record)=1;return commit_message(record,1);}
        if(value==2u){ACTOR_GROUP(record)=1;return commit_message(record,0);}
        if(value==3u){ACTOR_GROUP(record)=12;return commit_message(record,2);}
        ACTOR_GROUP(record)=1;return commit_message(record,3);
    case 3:case 23:
        cycle_counter(record,kind==3u?2u:4u,0);value=ACTOR_COUNTER(record);
        ACTOR_GROUP(record)=13;MESSAGE_RESULT=value;break;
    case 24:
        cycle_counter(record,2,0);value=ACTOR_COUNTER(record);ACTOR_GROUP(record)=14;MESSAGE_RESULT=value;break;
    case 98:ACTOR_GROUP(record)=8;return commit_message(record,0);
    case 73:return commit_message(record,ACTOR_STATE(record)==12u);
    case 25:
        state=ACTOR_STATE(record);
        if(state==8u||state==12u)return commit_message(record,2);
        if(state==9u||state==10u)return commit_message(record,0);
        if(state==11u||state==13u)return commit_message(record,1);
        goto existing_result;
    case 85:
        state=ACTOR_STATE(record);current=state>=15u&&state<=30u&&state!=24u&&state!=25u&&state!=27u&&state!=28u&&state!=29u;
        if((uint8_t)FUN_004bf9d0(0xd05720u))return commit_message(record,11);
        if(current){ACTOR_GROUP(record)=14;return commit_message(record,12);}
        goto existing_result;
    case 96:ACTOR_GROUP(record)=14;return commit_message(record,12);
    case 5:
        city=city_offset();
        if((int32_t)U(0xea446cu+city)>5)return commit_message(record,10);
        if((int32_t)U(0xea4474u+city)>5)return commit_message(record,11);
        goto existing_result;
    case 75:case 90:value=ACTOR_STATE(record)==11u;ACTOR_GROUP(record)=6;MESSAGE_RESULT=value;break;
    case 80:value=ACTOR_STATE(record)!=10u;ACTOR_GROUP(record)=14;MESSAGE_RESULT=value;break;
    case 79:case 81:case 108:value=ACTOR_STATE(record)!=9u;ACTOR_GROUP(record)=14;MESSAGE_RESULT=value;break;
    case 39:
        state=ACTOR_STATE(record);
        if(state==8u){ACTOR_GROUP(record)=6;return commit_message(record,10);}
        if(state==9u){ACTOR_GROUP(record)=6;return commit_message(record,11);}
        goto existing_result;
    case 26:value=ACTOR_STATE(record)!=2u;ACTOR_GROUP(record)=6;MESSAGE_RESULT=value;break;
    case 66:
        cycle_counter(record,3,0);value=ACTOR_COUNTER(record);ACTOR_GROUP(record)=6;MESSAGE_RESULT=value;break;
    case 4:
        state=ACTOR_STATE(record);
        if(state==8u){
            amount=B(0xbcd65eu+record);
            if(amount==2u){ACTOR_GROUP(record)=1;return commit_message(record,0);}
            if(amount==1u)return commit_message(record,1);
        }else if(state==9u||state==10u||state==11u){
            if(actor_target_distance(record)>=25u)return commit_message(record,2);
        }
        goto existing_result;
    case 9:
        state=ACTOR_STATE(record);
        if(state==8u){if(B(0xbcd65eu+record)==2u)return commit_message(record,1);}
        else if(state==9u){if(actor_target_distance(record)>=25u)return commit_message(record,2);}
        goto existing_result;
    case 38:
        state=ACTOR_STATE(record);
        if(state==11u||state==12u){
            if(actor_target_distance(record)>=25u){value=2;ACTOR_GROUP(record)=6;MESSAGE_RESULT=value;break;}
            value=MESSAGE_RESULT;ACTOR_GROUP(record)=6;break;
        }
        goto existing_result;
    case 20:{
        uint32_t resource;state=ACTOR_STATE(record);resource=B(0xbcd677u+record);
        if(state==13u){
            uint32_t offset=resource*88u;
            value=U(0x77af04u+offset);
            if(!value)value=U(0x77af00u+offset);
            value=value?4u:2u;ACTOR_GROUP(record)=6;MESSAGE_RESULT=value;break;
        }
        if(state==10u){
            value=FUN_00495b60((uint32_t)actor);
            if(value==1u){MESSAGE_RESULT=value;ACTOR_GROUP(record)=6;}
            else{ACTOR_GROUP(record)=6;value=value==2u?0u:2u;MESSAGE_RESULT=value;}
            break;
        }
        value=3;ACTOR_GROUP(record)=6;MESSAGE_RESULT=value;break;}
    case 19:case 21:{
        uint32_t resource,building,player,trader;
        cycle_counter(record,3,1);state=ACTOR_STATE(record);value=ACTOR_COUNTER(record);resource=B(0xbcd677u+record);
        ++value;MESSAGE_RESULT=value;
        if(state==11u){
            uint32_t offset=resource*88u,trade=U(0x77af04u+offset);
            if(!trade)trade=U(0x77af00u+offset);
            if(!trade){value=0;ACTOR_GROUP(record)=6;MESSAGE_RESULT=value;break;}
        }else if(state==10u){
            building=(uint32_t)(int32_t)(int16_t)W(0xbcd656u+record);player=U(0xeaac28u);trader=B(0xbcd666u+record);
            if(FUN_0045cf50(actor,(int32_t)player,(int32_t)building,(int32_t)trader)){
                value=4;ACTOR_GROUP(record)=6;MESSAGE_RESULT=value;break;
            }
            building=(uint32_t)(int32_t)(int16_t)W(0xbcd656u+record);player=U(0xeaac28u);trader=B(0xbcd666u+record);
            if(FUN_0045d180(actor,(int32_t)player,(int32_t)building,(int32_t)trader)){
                value=3;ACTOR_GROUP(record)=6;MESSAGE_RESULT=value;break;
            }
            value=MESSAGE_RESULT;
        }
        ACTOR_GROUP(record)=6;break;}
    case 91:ACTOR_GROUP(record)=11;return commit_message(record,10);
    case 27:
        value=FUN_004c5120((int16_t)W(0xbcd652u+record));
        switch(value){
        case 0:
            if((int32_t)U(0xd38374u)>=60||!B(0xd386dcu))goto existing_result;
            value=11;MESSAGE_RESULT=value;ACTOR_GROUP(record)=13;break;
        case 1:
            if((int32_t)U(0xea5b48u+city_offset())>=20)goto existing_result;
            ACTOR_GROUP(record)=5;return commit_message(record,11);
        case 2:
            if((int32_t)U(0xea4474u+city_offset())<=10)goto existing_result;
            ACTOR_GROUP(record)=1;return commit_message(record,11);
        case 3:
            if(!current)goto existing_result;
            ACTOR_GROUP(record)=2;return commit_message(record,11);
        case 4:
            if(FUN_004c52d0((int16_t)W(0xbcd652u+record),2)){ACTOR_GROUP(record)=8;return commit_message(record,13);}
            city=city_offset();
            if((int32_t)U(0xea1880u+city)<60){value=11;MESSAGE_RESULT=value;ACTOR_GROUP(record)=13;break;}
            if((int32_t)U(0xea5bf4u+city)>=60)goto existing_result;
            ACTOR_GROUP(record)=10;return commit_message(record,12);
        default:goto existing_result;
        }
        ACTOR_MESSAGE(record)=(uint8_t)value;return value;
    case 32:case 34:
        if((int32_t)U(0xea1888u+city_offset())>=(kind==32u?40:30))goto existing_result;
        ACTOR_GROUP(record)=8;return commit_message(record,10);
    case 33:
        amount=B(0xbcd65eu+record);ACTOR_GROUP(record)=8;
        /* Native compares a zero-extended BYTE with800. */
        value=(uint32_t)amount>800u?2u:1u;MESSAGE_RESULT=value;break;
    case 10:
        cycle_counter(record,13,0);
        if(ACTOR_STATE(record)!=13u)goto existing_result;
        value=ACTOR_COUNTER(record);MESSAGE_RESULT=value;break;
    case 8:
        amount=B(0xbcd65eu+record);
        /* Native SETGE compares a zero-extended BYTE with600. */
        return commit_message(record,(uint32_t)amount>=600u?11u:10u);
    case 88:{
        cycle_counter(record,4,0);
        if(ACTOR_STATE(record)==5u){
            uint8_t first,second;
            first=ACTOR_KIND((uint32_t)(int32_t)(int16_t)W(0xbcd67au+record)*388u);
            if(first==23u){value=13;MESSAGE_RESULT=value;ACTOR_GROUP(record)=13;break;}
            second=ACTOR_KIND((uint32_t)(int32_t)(int16_t)W(0xbcd67cu+record)*388u);
            if(second==23u||first==22u||second==22u){value=13;MESSAGE_RESULT=value;ACTOR_GROUP(record)=13;break;}
            value=ACTOR_COUNTER(record)+14u;MESSAGE_RESULT=value;break;
        }
        amount=B(0xbcd65eu+record);
        if((uint32_t)amount>=500u){value=12;MESSAGE_RESULT=value;ACTOR_GROUP(record)=13;break;}
        if(amount<100u)return commit_message(record,10);
        value=11;MESSAGE_RESULT=value;ACTOR_GROUP(record)=13;break;}
    case 7:
        if(employment_percent(city_offset())<50){value=10;ACTOR_GROUP(record)=12;MESSAGE_RESULT=value;break;}
        amount=B(0xbcd65eu+record);
        if(amount>=7u){value=11;ACTOR_GROUP(record)=12;MESSAGE_RESULT=value;break;}
        if(amount>=1u){value=12;MESSAGE_RESULT=value;ACTOR_GROUP(record)=12;break;}
        value=MESSAGE_RESULT;ACTOR_GROUP(record)=12;break;
    case 89:
        if(U(0xea6090u+city_offset()))return commit_message(record,11);
        amount=B(0xbcd65eu+record);
        if((uint32_t)amount>=500u){value=14;MESSAGE_RESULT=value;ACTOR_GROUP(record)=13;break;}
        if(amount>=100u){value=13;MESSAGE_RESULT=value;ACTOR_GROUP(record)=13;break;}
        return commit_message(record,12);
    case 78:
        city=city_offset();value=U(0xea5ea4u+city);
        if(value){
            if(ACTOR_STATE(record)==10u){value=0;ACTOR_GROUP(record)=3;MESSAGE_RESULT=value;break;}
            if(ACTOR_STATE(record)==12u){value=1;ACTOR_GROUP(record)=3;MESSAGE_RESULT=value;break;}
        }
        if(U(0xea6210u+city)){
            state=ACTOR_STATE(record);
            if(state!=10u&&state!=12u){value=2;ACTOR_GROUP(record)=3;MESSAGE_RESULT=value;break;}
        }
        value=current?3u:4u;ACTOR_GROUP(record)=3;MESSAGE_RESULT=value;break;
    case 77:
        city=city_offset();value=U(0xea5ea4u+city);
        if(value&&ACTOR_STATE(record)==10u){value=0;ACTOR_GROUP(record)=3;MESSAGE_RESULT=value;break;}
        if(U(0xea6210u+city)){value=1;ACTOR_GROUP(record)=3;MESSAGE_RESULT=value;break;}
        value=current?2u:3u;ACTOR_GROUP(record)=3;MESSAGE_RESULT=value;break;
    case 42:
        value=U(0xea5ea4u+city_offset());
        if(!value){ACTOR_GROUP(record)=2;return commit_message(record,value);}
        if(current){ACTOR_GROUP(record)=2;return commit_message(record,1);}
        if((int32_t)value<=10){ACTOR_GROUP(record)=2;return commit_message(record,2);}
        if((int32_t)value<=30){ACTOR_GROUP(record)=2;return commit_message(record,3);}
        goto existing_result;
    case 15:case 16:case 17:case 18:case 29:case 30:case 31:case 40:case 87:case 105:
        goto existing_result;
    default:
        ACTOR_MESSAGE(record)=255;MESSAGE_RESULT=UINT32_MAX;return kind-1u;
    }
    return finish_message(record,value);
existing_result:
    return finish_message(record,MESSAGE_RESULT);
}
void restore_actor_update_message(int32_t actor)
{(void)restore_actor_update_message_result(actor,0);}
