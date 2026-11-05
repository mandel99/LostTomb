#include "actor_route_queries_internal.h"

/* Whole 47E340, repaired in its original owner. Building records are 264
   bytes and their kind is a WORD; the actor kind is sampled once as a BYTE.
   Initial terrain flags are read before building filtering. */
uint32_t restore_actor_prune_service_road_exits(int32_t tile,uint32_t actor)
{
    uint32_t cell=(uint32_t)tile,flags[4],count=0,i,channel;
    uint32_t kind=(uint32_t)RQ_B(0xbcd602u+actor*388u);
    int32_t building_kind,minimum=7,cost[4];
    RQ_U(0xbc97dcu)=0;RQ_U(0xbc97d4u)=0;
    flags[2]=RQ_U(0xaf5d1cu+cell*4u);
    flags[0]=RQ_U(0xaf55fcu+cell*4u);
    RQ_U(0xbc97ccu)=0;RQ_U(0xbc97c4u)=0;
    flags[1]=RQ_U(0xaf5990u+cell*4u);
    RQ_U(0xbc97d0u)=flags[2]&0x440u;
    flags[3]=RQ_U(0xaf5988u+cell*4u);
    RQ_U(0xbc97c0u)=flags[0]&0x440u;
    RQ_U(0xbc97c8u)=flags[1]&0x440u;
    RQ_U(0xbc97d8u)=flags[3]&0x440u;
    for(i=0;i<4u;++i){
        static const uint32_t building_map[4]={0xb4e534u,0xb4e6feu,0xb4e8c4u,0xb4e6fau};
        if(flags[i]&8u){
            building_kind=RQ_H(0x7999e8u+(uint32_t)RQ_H(building_map[i]+cell*2u)*264u);
            if(building_kind==0xab||building_kind==0xca||building_kind==0x88||
               (building_kind==0x8a&&kind!=0x62u))RQ_U(0xbc97c0u+i*8u)=0;
        }
    }
    for(i=0;i<4u;++i)if(RQ_U(0xbc97c0u+i*8u))++count;
    if(count<3u)return count;
    channel=RQ_U(0x5dbf70u+kind*4u);
    for(i=0;i<4u;++i){
        if(!RQ_U(0xbc97c0u+i*8u))continue;
        cost[i]=RQ_I(FUN_0047e230(channel,RQ_I(cell+RQ_U(0x5dc128u+i*8u))));
        if(cost[i]<=minimum)minimum=cost[i];
    }
    for(i=0;i<4u;++i){
        if(RQ_U(0xbc97c0u+i*8u)&&minimum<cost[i]){
            RQ_U(0xbc97c0u+i*8u)=0;--count;
        }
    }
    return count;
}
uint32_t FUN_0047e340(int32_t tile,uint32_t actor)
{return restore_actor_prune_service_road_exits(tile,actor);}
