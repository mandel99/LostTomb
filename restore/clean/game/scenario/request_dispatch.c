#include "pharaoh/scenario/request_dispatch.h"
#include <stddef.h>

_Static_assert(offsetof(PharaohScenarioEvent,item)==4 &&
    offsetof(PharaohScenarioEvent,amount)==12 &&
    offsetof(PharaohScenarioEvent,months_left)==46 &&
    offsetof(PharaohScenarioEvent,state)==48 &&
    offsetof(PharaohRequestDispatchRandom,first_15)==0x1a0 &&
    offsetof(PharaohRequestDispatchRandom,second_7)==0x1b0 &&
    offsetof(PharaohRequestDispatchRandom,seed_one)==0x1bc &&
    offsetof(PharaohRequestDispatchRandom,seed_two)==0x1c0 &&
    sizeof(PharaohRequestDispatchRandom)==0x1c4,
    "native request and inline random fields");

static int16_t delivery_months(PharaohRequestDispatchRandom *r)
{
    r->history[r->history_index]=r->second_7;
    if(++r->history_index>=100) r->history_index=0;
    uint32_t one=r->seed_one,two=r->seed_two;
    /* Reuse the existing31-step feedback algorithm with native storage. */
    for(int32_t i=0;i<31;++i){
        const uint32_t f1=((one>>4)^one)&1;
        const uint32_t f2=((two>>4)^two)&1;
        one>>=1;two>>=1;
        if(f1!=0) one|=UINT32_C(0x40000000);
        if(f2!=0) two|=UINT32_C(0x40000000);
    }
    r->seed_two=two;
    r->second_3=two&7;
    r->first_15=one&0x7fff;
    r->second_15=two&0x7fff;
    r->seed_one=one;
    r->second_7=two&0x7f;
    r->first_7=one&0x7f;
    r->first_3=one&7;
    return (int16_t)((two&3)+1);
}

void pharaoh_scenario_request_dispatch(const PharaohRequestDispatchContext *c,
    int32_t event_id)
{
    if(event_id<0 || event_id>=*c->event_count) return;
    PharaohScenarioEvent *const event=&c->events[event_id];
    const int32_t amount=event->amount.value;
    event->state=(int16_t)((event->state!=0)+2);
    const int32_t item=event->item.value;
    if(item==36){
        event->months_left=delivery_months(c->random);
        *c->treasury-=(uint32_t)amount;
        *c->request_expenses+=(uint32_t)amount;
    }else if(item==37){
        event->state=5;
        event->months_left=(int16_t)pharaoh_request_travel_months(c->travel,event);
        pharaoh_formation_legions_dispatch_to_request(c->military,event->id);
        if(event->months_left<=0) event->months_left=12;
    }else{
        event->months_left=delivery_months(c->random);
        /* Native WORD amount*100 cannot overflow signed32. */
        (void)pharaoh_storage_withdraw_city(c->storage,item,amount*100);
    }
}
