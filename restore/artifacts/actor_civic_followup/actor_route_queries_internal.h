#ifndef RESTORE_ACTOR_ROUTE_QUERIES_INTERNAL_H
#define RESTORE_ACTOR_ROUTE_QUERIES_INTERNAL_H
#include "actor_route_queries.h"
#include "actor_order_path.h"
#include "actor_routing.h"
#include "actor_walking.h"
#include "actor_services.h"
#define RQ_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define RQ_H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define RQ_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define RQ_C(a) ((int32_t)*(volatile int8_t *)(uintptr_t)(uint32_t)(a))
#define RQ_I(v) ((int32_t)(uint32_t)(v))

#ifdef RESTORE_ROUTE_QUERY_CARDINAL
/* 51D420 and 51D6D0 have the same ordered cardinal expansion. Keep the
   distance snapshot before callbacks, and the cached tail across visited
   neighbors. A rejected checked neighbor reloads the live tail. */
static __inline uint32_t restore_route_query_expand_cardinal(uint32_t current,uint32_t tail)
{
    static const int32_t delta[4]={-228,1,228,-1};
    uint32_t cost=RQ_U(0xeed760u+current*4u)+1u,i,next;
    int32_t classification;
    for(i=0;i<4u;++i){
        next=current+(uint32_t)delta[i];
        if(RQ_U(0xeed760u+next*4u))continue;
        if(!FUN_0051aff0(RQ_I(next))){tail=RQ_U(0xeae008u);continue;}
        classification=RQ_C(0xee0c50u+next);
        if((classification<0&&classification!=-3&&
            (classification!=-1||!(RQ_U(0xaf598cu+next*4u)&0x1000u)))||
           (classification>4&&classification!=6)){
            tail=RQ_U(0xeae008u);continue;
        }
        RQ_U(0xeed760u+next*4u)=cost;
        tail=RQ_U(0xeae008u);RQ_U(0xf203a0u+tail*4u)=next;
        ++tail;RQ_U(0xeae008u)=tail;
        if(RQ_I(tail)>51983){tail=0;RQ_U(0xeae008u)=0;}
    }
    return tail;
}
#endif
#endif
