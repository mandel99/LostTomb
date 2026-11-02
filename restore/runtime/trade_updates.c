#include "trade_updates.h"
#include "menu_training_render.h"
#include "training_queries.h"
#include "actor_create.h"
#include "event_publisher.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
static uint32_t player_offset(void){return U(0xeaac28u)*0x49d8u;}
static void cooldown(uint32_t address)
{if(H(address)>0)W(address)=(uint16_t)(W(address)-1u);else W(address)=0;}
static uint32_t caravan_actor(int32_t kind)
{
    uint32_t owner=U(0xeaac28u),offset=owner*0x49d8u;
    return (uint32_t)FUN_004b4800((uint8_t)owner,kind,H(0xea3fe8u+offset),H(0xea3feau+offset),0);
}

/* Complete444C20/333 instructions. Country records are106 bytes and actor
   records388 bytes. Preserve the initial city's cached counters, subsequent
   live city reads, per-country notification/timer order and first-spawn exit. */
void FUN_00444c20(void)
{
    int32_t counts[16]={0},port_x;
    uint32_t initial=player_offset(),record,p,resource,candidates,weight,fee,required;
    uint32_t slot,owner,offset,actor,follower,last,address;
    W(0xea5cecu+initial)=0;W(0xea5ceeu+initial)=0;U(0xea5f14u+initial)=0;
    if(I(U(0x78d728u))>0)counts[0]=1;
    required=(uint32_t)FUN_00559fc0(counts);
    U(0xea5f14u+initial)+=required;
    cooldown(0xea5cf0u+initial);cooldown(0xea5cf2u+initial);
    for(record=1,p=0x77de6fu;p<0x77f747u;++record,p+=106u){
        if(!B(p-5u)||!B(p))continue;
        if(B(p+83u)==1u){
            if(I(U(0x705c1cu))<=0){
                if(I(U(0xe9519cu))<=0){
                    U(0xe950dcu)=1;FUN_004e0cf0(316,0,0,0,0);U(0xe9519cu)=384;
                }else --U(0xe9519cu);
                continue;
            }
            if(W(0x784c8cu)==UINT16_MAX&&W(0x784c8eu)==UINT16_MAX)continue;
            ++W(0xea5cecu+player_offset());
        }else ++W(0xea5ceeu+player_offset());
        candidates=weight=0;
        for(resource=1;resource<36u;++resource){
            if((B(p+resource+1u)&&(uint8_t)FUN_00447790(I(record),I(resource)))||
               (B(p+resource+37u)&&(uint8_t)FUN_00447790(I(record),I(resource)))){
                ++candidates;
                fee=U(0x77646cu+((uint32_t)B(p-1u)*36u+resource)*4u);
                if(I(fee)>25)weight+=3u;else if(I(fee)>15)weight+=2u;else if(I(fee)>0)++weight;
            }
        }
        required=candidates>1u?weight/candidates+(weight%candidates!=0u):weight;
        if(!required||W(p+79u))continue;
        for(slot=0;slot<required&&slot<3u;++slot)if(!W(p+85u+slot*2u))break;
        if(slot>=required||slot>=3u)continue;
        if(H(p+77u)>0){W(p+77u)=(uint16_t)(W(p+77u)-1u);continue;}
        W(p+77u)=B(p+83u)==1u?30u:4u;
        if(B(p+83u)==1u){
            if(I(U(0x705c1cu))<=0)continue;
            port_x=H(0x784c8cu);
            if(port_x==-1&&H(0x784c8eu)==-1)continue;
            owner=U(0xeaac28u);offset=owner*0x49d8u;
            if(W(0xea5cf0u+offset)||I(U(0xea6024u+offset))>0)continue;
            actor=(uint32_t)FUN_004b4800((uint8_t)owner,20,port_x,H(0x784c8eu),0);
            address=actor*388u;
            W(0xbcd66cu+address)=12;B(0xbcd667u+address)=0;B(0xbcd666u+address)=(uint8_t)record;
            B(0xbcd630u+address)=8;W(0xbcd62eu+address)=10;
            W(p+85u+slot*2u)=(uint16_t)actor;B(0xbcd672u+address)=0;return;
        }
        offset=player_offset();if(W(0xea5cf2u+offset)||I(U(0xea6024u+offset))>0)continue;
        actor=caravan_actor(19);address=actor*388u;
        W(p+85u+slot*2u)=(uint16_t)actor;
        B(0xbcd666u+address)=(uint8_t)record;B(0xbcd630u+address)=8;W(0xbcd62eu+address)=10;
        follower=caravan_actor(21);address=follower*388u;
        B(0xbcd630u+address)=8;W(0xbcd660u+address)=(uint16_t)actor;
        last=caravan_actor(21);address=last*388u;
        B(0xbcd630u+address)=8;W(0xbcd660u+address)=(uint16_t)follower;return;
    }
}
