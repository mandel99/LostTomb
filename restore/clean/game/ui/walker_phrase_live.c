#include "pharaoh/ui/walker_phrase_live.h"
#include "pharaoh/ui/city_undo_live.h"
#include "pharaoh/core/static_objects.h"
#include <stddef.h>
#include <limits.h>

_Static_assert(sizeof(PharaohWalkerPhrasePlayer)==0x49d8,"native player stride");
_Static_assert(offsetof(PharaohWalkerPhrasePlayer,trade_state)==0x281a &&
    offsetof(PharaohWalkerPhrasePlayer,resource_permissions)==0x28aa &&
    offsetof(PharaohWalkerPhrasePlayer,labor)==0x2b20 &&
    offsetof(PharaohWalkerPhrasePlayer,native_ea5c0a)==0x438e &&
    offsetof(PharaohWalkerPhrasePlayer,resource_stock)==0x48e4 &&
    offsetof(PharaohWalkerPhrasePlayer,native_ea61f4)==0x4978 &&
    offsetof(PharaohWalkerPhrasePlayer,native_ea6210)==0x4994,
    "native player phrase/trade offsets");
_Static_assert(sizeof(PharaohWalkerPhraseTrader)==106 &&
    offsetof(PharaohWalkerPhraseTrader,route)==4 &&
    offsetof(PharaohWalkerPhraseTrader,imports)==42,"native external trader view");
_Static_assert(sizeof(PharaohWalkerPhraseResource)==88,"native resource stride");
_Static_assert(sizeof(PharaohWalkerQueryRecord)==388 &&
    offsetof(PharaohWalkerQueryRecord,reserved_07c)==0x7c &&
    offsetof(PharaohWalkerQueryRecord,reserved_131)==0x131 &&
    offsetof(PharaohWalkerQueryRecord,reserved_066)==0x66 &&
    offsetof(PharaohWalkerQueryRecord,reserved_06e)==0x6e,
    "native walker phrase overlays");
_Static_assert(sizeof(PharaohScenarioEvent)==124 && sizeof(PharaohLaborCategory)==20,
    "native event and labor strides");

#define Counter(w) ((w)->reserved_07c[0])
#define Message(w) ((w)->reserved_07c[1])
#define Group(w) ((w)->reserved_131)
#define Amount(w) ((w)->reserved_066[0])

static volatile PharaohWalkerPhrasePlayer *player(const PharaohWalkerPhraseContext *c)
{return &c->players[*c->current_player];}
static PharaohScenarioEvent *next_invasion(const PharaohWalkerPhraseContext *c,
    const PharaohScenarioEvent *previous)
{
    return pharaoh_scenario_invasion_next(c->events,*c->event_count,previous,false,
        c->invasion_search_flag,c->current_event_index);
}
static uint32_t invasion_running(const PharaohWalkerPhraseContext *c)
{
    PharaohScenarioEvent *event=next_invasion(c,NULL);
    while(event!=NULL){
        if(pharaoh_event_is_running(event,c->calendar)!=0)return 1;
        event=next_invasion(c,event);
    }
    return 0;
}

/* These are native inline signed-IDIV sites, unlike4CDE60's zero-denominator
 * return. Preserve wrapped numerator and report both actual#DE conditions. */
static PharaohWalkerPhraseStatus signed_percent(uint32_t amount,uint32_t total,
    int32_t *result)
{
    const int32_t numerator=(int32_t)(amount*100u),denominator=(int32_t)total;
    if(denominator==0)return PHARAOH_WALKER_PHRASE_DIVIDE_BY_ZERO;
    if(numerator==INT32_MIN && denominator==-1)return PHARAOH_WALKER_PHRASE_DIVIDE_OVERFLOW;
    *result=numerator/denominator;return PHARAOH_WALKER_PHRASE_VALUE;
}

/* Complete existing MessageCategory C,263 native instructions. Category2's
 * sticky preceding weight and the final mutually suppressing3/6 slots remain. */
PharaohWalkerPhraseResult pharaoh_walker_phrase_build_weights(
    const PharaohWalkerPhraseContext *c,volatile uint8_t weights[8],
    volatile uint8_t *all_defaults)
{
    PharaohWalkerPhraseResult result={PHARAOH_WALKER_PHRASE_VALUE,0};
    *(volatile uint32_t *)(volatile void *)weights=0;
    *(volatile uint32_t *)(volatile void *)(weights+4)=0;
    uint32_t blocked=0,weight=0,sum=0;
    PharaohScenarioEvent *event=next_invasion(c,NULL);
    while(event!=NULL){
        if((event->flags&0x10u)!=0){blocked=(event->flags>>2)&1u;break;}
        event=next_invasion(c,event);
    }
    const uint32_t current=invasion_running(c);
    for(uint32_t index=0;index<8;++index){
        uint32_t clear=0;int32_t value;
        switch(index){
        case 0:
            value=player(c)->native_ea1888;
            weight=value<20?8u:value<30?6u:value<40?4u:1u;clear=weight!=1;break;
        case 1:
            value=player(c)->native_ea4388;
            weight=value<1?6u:value<2?4u:1u;clear=weight!=1;break;
        case 2:
            if(current==0)weight=1;
            else if(*c->native_bcb9b4==0 && player(c)->native_ea5ed8==0){weight=8;clear=1;}
            break;
        case 3:{
            volatile PharaohWalkerPhrasePlayer *const p=player(c);
            uint32_t allocated=0,demand=0;
            for(uint32_t i=0;i<10;++i)allocated+=(uint32_t)p->labor[i].allocated;
            for(uint32_t i=0;i<10;++i)demand+=(uint32_t)p->labor[i].demand;
            if(demand==0)weight=1;
            else{
                result.status=signed_percent(allocated,demand,&value);
                if(result.status!=PHARAOH_WALKER_PHRASE_VALUE)return result;
                value=(int32_t)(100u-(uint32_t)value);
                weight=value>20?8u:value>10?6u:value>5?4u:1u;
            }
            clear=weight!=1;break;
        }
        case 4:{
            volatile PharaohWalkerPhrasePlayer *const p=player(c);
            int32_t minimum=100;
            for(uint32_t i=0;i<5;++i){value=p->native_ea5c0a[i];if(value<minimum)minimum=value;}
            weight=minimum<=20?6u:minimum<=30?4u:1u;clear=weight!=1;break;
        }
        case 5:
            if(blocked!=0)weight=0;
            else{value=player(c)->native_ea5b48;weight=value<=0?8u:value<20?6u:value<40?4u:1u;clear=weight!=1;}
            break;
        case 6:{
            volatile PharaohWalkerPhrasePlayer *const p=player(c);
            const uint32_t total=p->native_ea1894;
            if(total==0)weight=1;
            else{
                result.status=signed_percent(p->native_ea4468,total,&value);
                if(result.status!=PHARAOH_WALKER_PHRASE_VALUE)return result;
                weight=value>17?8u:value>10?6u:value>5?4u:1u;
            }
            clear=weight!=1;break;
        }
        case 7:
            value=player(c)->native_ea5bf4;weight=value<=0?6u:value<20?4u:1u;clear=weight!=1;break;
        }
        if(clear!=0)*all_defaults=0;
        sum+=weight;weights[index]=(uint8_t)weight;
    }
    const uint8_t third=weights[3];
    if(third==1 && weights[6]>=1){weights[3]=0;result.value=sum-1u;return result;}
    if(third>1 && weights[6]==1){weights[6]=0;--sum;}
    result.value=sum;return result;
}

/* Complete63-instruction53C060. Native selector is bitwise AND, not modulo.
 * The producer's fullDWORD total goes to IDIV; no incoming EAX is needed. */
PharaohWalkerPhraseResult pharaoh_walker_phrase_choose_category(const PharaohWalkerPhraseContext *c)
{
    uint8_t all_defaults=1;
    PharaohWalkerPhraseResult result=pharaoh_walker_phrase_build_weights(c,c->weights,&all_defaults);
    if(result.status!=PHARAOH_WALKER_PHRASE_VALUE)return result;
    const uint32_t total=result.value;
    uint32_t percent_sum=0,cumulative=0,index;int32_t percentage;
    for(index=0;index<8;++index){
        result.status=signed_percent(c->weights[index],total,&percentage);
        if(result.status!=PHARAOH_WALKER_PHRASE_VALUE)return result;
        percent_sum+=(uint32_t)percentage;
    }
    const uint32_t selector=*c->random_second_7&percent_sum;
    if(all_defaults!=0){result.value=9;return result;}
    for(index=0;index<8;++index){
        result.status=signed_percent(c->weights[index],total,&percentage);
        if(result.status!=PHARAOH_WALKER_PHRASE_VALUE)return result;
        cumulative+=(uint32_t)percentage;
        if((int32_t)selector<=(int32_t)cumulative)break;
    }
    if(index==8)index=7;
    result.value=c->weights[index]==1?8u:index;return result;
}

/* Actual-layout adaptation of the existing42-instruction processing scan.
 * The existing4894A0 wrapper only supplies building X/Y to489470, so borrow
 * those actual264-byte fields directly instead of constructing another pool. */
uint8_t pharaoh_walker_phrase_processing_blocked(const PharaohWalkerPhraseContext *c)
{
    const uint32_t mode=*c->processing_mode;
    if(mode==1 || mode==2 || mode==3)return 0;
    for(int32_t id=1;id<4000;++id){
        const PharaohBuildingTypeRecord *const b=&c->buildings[id];
        if(b->state!=1)continue;
        if((uint8_t)pharaoh_building_is_farm(c->buildings,id)==0)continue;
        if((uint8_t)pharaoh_region_has_active_occupant(c->occupancy,b->x,b->y,3,3)==0)continue;
        if(b->runtime_value_b4!=0)continue;
        const volatile PharaohWalkerPhrasePlayer *const p=player(c);
        if(pharaoh_building_resource_is_permitted(c->buildings,id,
            (const int16_t *)(const void *)p->resource_permissions)==0)return 1;
    }
    return 0;
}

static int32_t adjusted_quota(const PharaohWalkerPhraseContext *c,int32_t value)
{
    const volatile PharaohWalkerPhrasePlayer *const p=player(c);
    const int16_t first=p->native_ea61f4,second=p->native_ea61f6,third=p->native_ea61f8;
    return pharaoh_gods_ra_adjust_value(value,first,second,third);
}
static uint16_t trade_state(const volatile PharaohWalkerPhrasePlayer *p,int32_t resource)
{
    /* Original reads this WORD before its resource-domain guard. */
    const uintptr_t at=(uintptr_t)p+offsetof(PharaohWalkerPhrasePlayer,trade_state)+
        (uint32_t)resource*2u;
    return *(const volatile uint16_t *)at;
}

/* Whole107/114-instruction existing StorageTrade C; all quota adjustment is
 * the real canonical4C5240 with actual three selected-player WORD inputs. */
uint32_t pharaoh_walker_phrase_trade_can_export(const PharaohWalkerPhraseContext *c,
    uint32_t owner,int32_t trader,int32_t resource)
{
    const volatile PharaohWalkerPhrasePlayer *const p=&c->players[owner];
    if(trade_state(p,resource)!=2 && trade_state(p,resource)!=4)return 0;
    if(resource==27 || resource<0 || resource>=36)return 0;
    const uint32_t table=(uint32_t)resource+(uint32_t)c->traders[trader].route*36u;
    const int32_t quota=adjusted_quota(c,c->route_limits[table]);
    if(trader!=0){
        const uint32_t fresh=(uint32_t)resource+(uint32_t)c->traders[trader].route*36u;
        if(c->route_traded[fresh]>=quota)return 0;
    }
    const int32_t stock=p->resource_stock[resource];
    if(trade_state(p,resource)==2){if(stock<=(int32_t)p->trade_keep[resource]*100)return 0;}
    else if(stock<=c->export_keep[resource])return 0;
    if(trader!=0 && c->traders[trader].exports[resource]==0)return 0;
    return 1;
}
uint32_t pharaoh_walker_phrase_trade_can_import(const PharaohWalkerPhraseContext *c,
    uint32_t owner,int32_t trader,int32_t resource)
{
    if(player(c)->native_ea187c<=-5000)return 0;
    const volatile PharaohWalkerPhrasePlayer *const p=&c->players[owner];
    if(trade_state(p,resource)!=1 && trade_state(p,resource)!=3)return 0;
    if(resource==27 || resource<0 || resource>=36)return 0;
    if(c->traders[trader].imports[resource]==0)return 0;
    const uint32_t table=(uint32_t)resource+(uint32_t)c->traders[trader].route*36u;
    const int32_t quota=adjusted_quota(c,c->route_limits[table]);
    const uint32_t fresh=(uint32_t)resource+(uint32_t)c->traders[trader].route*36u;
    if(c->route_traded[fresh]>=quota)return 0;
    const int32_t stock=p->resource_stock[resource];
    if(resource<1)return 0;
    if(trade_state(p,resource)==1)return stock<(int32_t)p->trade_keep[resource]*100;
    return stock<c->export_keep[resource];
}

/* Whole66-instruction45CF50, including the original eight links and slot0. */
uint32_t pharaoh_walker_phrase_storage_can_export(const PharaohWalkerPhraseContext *c,
    int32_t walker,uint32_t owner,int32_t building,int32_t trader)
{
    int32_t node=building;
    const int32_t kind=c->buildings[node].building_type;
    if((kind!=72 && kind!=193) || c->walkers[walker].reserved_06e[1]>=8)return 0;
    for(uint32_t i=0;i<8;++i){
        node=c->buildings[node].next_storage_building_id;if(node<=0)continue;
        if(c->buildings[node].loads_stored_render<=0)continue;
        const int32_t resource=c->buildings[node].subtype;
        if(pharaoh_walker_phrase_trade_can_export(c,owner,trader,resource)!=0 &&
            pharaoh_storageyard_resource_amount(c->buildings,building,c->buildings[node].subtype)>=100)return 1;
    }
    return 0;
}

/* Whole160-instruction45D180. The parameter player's cursor and the live
 * selected player's policy index intentionally remain distinct. */
uint32_t pharaoh_walker_phrase_storage_can_import(const PharaohWalkerPhraseContext *c,
    int32_t walker,uint32_t owner,int32_t building,int32_t trader)
{
    int32_t node=building;
    const int32_t kind=c->buildings[node].building_type;
    if(kind!=72 && kind!=193)return 0;
    /* WORD numerator*100 cannot overflow IDIV for any DWORD denominator;
     * denominator0 is the real4CDE60 zero-return case. Result is discarded. */
    (void)pharaoh_scale_value_to_percent(c->buildings[node].num_workers,c->models[kind].laborers);
    if((int16_t)c->walkers[walker].destination_payload_074>=8)return 0;
    const uint32_t slot=c->buildings[node].storage_id;
    int32_t eligible=0;
    for(int32_t resource=1;resource<36;++resource){
        const uint8_t policy=c->storages[slot].resource_state[resource];
        if(policy!=1 && policy!=3 &&
            pharaoh_city_undo_storage_limit_exceeded(c->buildings,c->storages,building,0,resource)==0 &&
            pharaoh_city_undo_storage_limit_exceeded(c->buildings,c->storages,building,100,resource)==0 &&
            pharaoh_walker_phrase_trade_can_import(c,owner,trader,resource)!=0)++eligible;
    }
    if(eligible<=0)return 0;
    volatile uint32_t *const cursor=&c->players[owner].native_ea5b34;
    const uint32_t allowed=pharaoh_walker_phrase_trade_can_import(c,owner,trader,(int32_t)*cursor);
    uint8_t policy=c->storages[slot].resource_state[player(c)->native_ea5b34];
    if(policy==1 || allowed==0){
        uint32_t i;
        for(i=1;i<36;++i){
            *cursor=*cursor+1u;if((int32_t)*cursor>=36)*cursor=1;
            policy=c->storages[slot].resource_state[player(c)->native_ea5b34];
            if(policy!=1 && pharaoh_walker_phrase_trade_can_import(c,owner,trader,(int32_t)*cursor)!=0)break;
        }
        if(i==36)return 0;
    }
    for(uint32_t i=0;i<8;++i){
        node=c->buildings[node].next_storage_building_id;if(node<=0)continue;
        const int16_t amount=c->buildings[node].loads_stored_render;
        if(amount<400 && (amount==0 || pharaoh_walker_phrase_trade_can_import(c,owner,trader,c->buildings[node].subtype)!=0))return 1;
    }
    return 0;
}

static void cycle_counter(volatile PharaohWalkerQueryRecord *w,uint32_t limit,uint8_t reset)
{
    const uint8_t next=(uint8_t)(Counter(w)+1u);Counter(w)=next;
    if(next>=limit)Counter(w)=reset;
}
static uint32_t target_distance(const PharaohWalkerPhraseContext *c,
    const volatile PharaohWalkerQueryRecord *w)
{
    const int32_t target_x=w->source_x,x=w->destination_x;
    const int32_t target_y=w->source_y,y=w->destination_y;
    const uint32_t dx=(uint32_t)(x>target_x?x-target_x:target_x-x);
    const uint32_t dy=(uint32_t)(y>target_y?y-target_y:target_y-y);
    *c->distance_x=dx;*c->distance_y=dy;
    return dx>=dy?dx:dy;
}
static PharaohWalkerPhraseStatus commit(const PharaohWalkerPhraseContext *c,
    volatile PharaohWalkerQueryRecord *w,uint32_t value)
{
    Message(w)=(uint8_t)value;*c->shared_result=value;return PHARAOH_WALKER_PHRASE_VALUE;
}
static PharaohWalkerPhraseStatus finish(const PharaohWalkerPhraseContext *c,
    volatile PharaohWalkerQueryRecord *w,uint32_t value)
{
    static const uint8_t groups[8]={8,13,2,1,11,5,1,10};
    if(value==UINT32_MAX){
        switch(w->type){
        case 5:case 7:case 10:case 15:case 16:case 17:case 18:case 27:
        case 29:case 30:case 31:case 32:case 34:case 35:case 39:case 40:
        case 87:case 88:case 89:case 105:{
            const PharaohWalkerPhraseResult selected=pharaoh_walker_phrase_choose_category(c);
            if(selected.status!=PHARAOH_WALKER_PHRASE_VALUE)return selected.status;
            value=selected.value;*c->shared_result=value;
            Group(w)=value==9 || value==8?13:groups[value];break;
        }
        default:break;
        }
    }
    Message(w)=(uint8_t)value;return PHARAOH_WALKER_PHRASE_VALUE;
}
static PharaohWalkerPhraseStatus employment_percent(
    const volatile PharaohWalkerPhrasePlayer *p,int32_t *value)
{
    const uint32_t a=p->native_ea4494,b=p->native_ea4490;
    const uint32_t c=p->native_ea449c,d=p->native_ea4498;
    return signed_percent(a+b,c+d+a+b,value);
}

/* Complete976-instruction ActorMessage C. Ordinary C case groups preserve
 * native cached values, BYTE publications and fresh owner/building/trader
 * reads. The4F7E50 caller consumes effects only; no incoming-EAX API exists. */
PharaohWalkerPhraseStatus pharaoh_walker_phrase_update(const PharaohWalkerPhraseContext *c,int32_t id)
{
    if(id<=0)return PHARAOH_WALKER_PHRASE_VALUE;
    volatile PharaohWalkerQueryRecord *const w=&c->walkers[id];
    Message(w)=0;*c->shared_result=UINT32_MAX;
    uint32_t current=invasion_running(c),value=0;
    Group(w)=0;
    const uint32_t kind=w->type;
    uint8_t state,amount;
    switch(kind){
    case 1:cycle_counter(w,3,0);value=Counter(w);*c->shared_result=value;break;
    case 2:
        value=player(c)->native_ea5db0;
        if(value==1){Group(w)=1;return commit(c,w,1);}
        if(value==2){Group(w)=1;return commit(c,w,0);}
        if(value==3){Group(w)=12;return commit(c,w,2);}
        Group(w)=1;return commit(c,w,3);
    case 3:case 23:
        cycle_counter(w,kind==3?2:4,0);value=Counter(w);Group(w)=13;*c->shared_result=value;break;
    case 24:cycle_counter(w,2,0);value=Counter(w);Group(w)=14;*c->shared_result=value;break;
    case 98:Group(w)=8;return commit(c,w,0);
    case 73:return commit(c,w,w->action_state==12);
    case 25:
        state=w->action_state;
        if(state==8 || state==12)return commit(c,w,2);
        if(state==9 || state==10)return commit(c,w,0);
        if(state==11 || state==13)return commit(c,w,1);
        goto existing;
    case 85:
        state=w->action_state;
        current=state>=15 && state<=30 && state!=24 && state!=25 && state!=27 && state!=28 && state!=29;
        if(pharaoh_walker_phrase_processing_blocked(c)!=0)return commit(c,w,11);
        if(current!=0){Group(w)=14;return commit(c,w,12);}
        goto existing;
    case 96:Group(w)=14;return commit(c,w,12);
    case 5:{
        const volatile PharaohWalkerPhrasePlayer *const p=player(c);
        if(p->native_ea446c>5)return commit(c,w,10);
        if(p->native_ea4474>5)return commit(c,w,11);
        goto existing;
    }
    case 75:case 90:value=w->action_state==11;Group(w)=6;*c->shared_result=value;break;
    case 80:value=w->action_state!=10;Group(w)=14;*c->shared_result=value;break;
    case 79:case 81:case 108:value=w->action_state!=9;Group(w)=14;*c->shared_result=value;break;
    case 39:
        state=w->action_state;
        if(state==8){Group(w)=6;return commit(c,w,10);}
        if(state==9){Group(w)=6;return commit(c,w,11);}
        goto existing;
    case 26:value=w->action_state!=2;Group(w)=6;*c->shared_result=value;break;
    case 66:cycle_counter(w,3,0);value=Counter(w);Group(w)=6;*c->shared_result=value;break;
    case 4:
        state=w->action_state;
        if(state==8){
            amount=Amount(w);
            if(amount==2){Group(w)=1;return commit(c,w,0);}
            if(amount==1)return commit(c,w,1);
        }else if(state==9 || state==10 || state==11){
            if(target_distance(c,w)>=25)return commit(c,w,2);
        }
        goto existing;
    case 9:
        state=w->action_state;
        if(state==8){if(Amount(w)==2)return commit(c,w,1);}
        else if(state==9){if(target_distance(c,w)>=25)return commit(c,w,2);}
        goto existing;
    case 38:
        state=w->action_state;
        if(state==11 || state==12){
            if(target_distance(c,w)>=25){value=2;Group(w)=6;*c->shared_result=value;break;}
            value=*c->shared_result;Group(w)=6;break;
        }
        goto existing;
    case 20:{
        state=w->action_state;
        const uint32_t resource=w->trader_record_id;
        if(state==13){
            value=c->resources[resource].native_77af04;
            if(value==0)value=c->resources[resource].native_77af00;
            value=value!=0?4u:2u;Group(w)=6;*c->shared_result=value;break;
        }
        if(state==10){
            value=(uint32_t)pharaoh_trade_ship_dock_activity(c->walkers,c->buildings,id);
            if(value==1){*c->shared_result=value;Group(w)=6;}
            else{Group(w)=6;value=value==2?0u:2u;*c->shared_result=value;}
            break;
        }
        value=3;Group(w)=6;*c->shared_result=value;break;
    }
    case 19:case 21:{
        cycle_counter(w,3,1);state=w->action_state;value=Counter(w);
        const uint32_t resource=w->trader_record_id;
        ++value;*c->shared_result=value;
        if(state==11){
            uint32_t trade=c->resources[resource].native_77af04;
            if(trade==0)trade=c->resources[resource].native_77af00;
            if(trade==0){value=0;Group(w)=6;*c->shared_result=value;break;}
        }else if(state==10){
            int32_t building=w->entertainment_building_id;
            uint32_t owner=*c->current_player;
            int32_t trader=w->reserved_06e[0];
            if(pharaoh_walker_phrase_storage_can_export(c,id,owner,building,trader)!=0){
                value=4;Group(w)=6;*c->shared_result=value;break;
            }
            building=w->entertainment_building_id;owner=*c->current_player;trader=w->reserved_06e[0];
            if(pharaoh_walker_phrase_storage_can_import(c,id,owner,building,trader)!=0){
                value=3;Group(w)=6;*c->shared_result=value;break;
            }
            value=*c->shared_result;
        }
        Group(w)=6;break;
    }
    case 91:Group(w)=11;return commit(c,w,10);
    case 27:
        value=(uint32_t)pharaoh_building_group_index(c->buildings,w->building_id);
        switch(value){
        case 0:
            if(*c->native_d38374>=60 || *c->native_d386dc==0)goto existing;
            value=11;*c->shared_result=value;Group(w)=13;break;
        case 1:
            if(player(c)->native_ea5b48>=20)goto existing;
            Group(w)=5;return commit(c,w,11);
        case 2:
            if(player(c)->native_ea4474<=10)goto existing;
            Group(w)=1;return commit(c,w,11);
        case 3:
            if(current==0)goto existing;
            Group(w)=2;return commit(c,w,11);
        case 4:{
            if(pharaoh_temple_upgrade_present(c->buildings,w->building_id,2)){Group(w)=8;return commit(c,w,13);}
            const volatile PharaohWalkerPhrasePlayer *const p=player(c);
            if(p->native_ea1880<60){value=11;*c->shared_result=value;Group(w)=13;break;}
            if(p->native_ea5bf4>=60)goto existing;
            Group(w)=10;return commit(c,w,12);
        }
        default:goto existing;
        }
        Message(w)=(uint8_t)value;return PHARAOH_WALKER_PHRASE_VALUE;
    case 32:case 34:
        if(player(c)->native_ea1888>=(kind==32?40:30))goto existing;
        Group(w)=8;return commit(c,w,10);
    case 33:
        amount=Amount(w);Group(w)=8;
        value=(uint32_t)amount>800u?2u:1u;*c->shared_result=value;break;
    case 10:
        cycle_counter(w,13,0);
        if(w->action_state!=13)goto existing;
        value=Counter(w);*c->shared_result=value;break;
    case 8:
        amount=Amount(w);return commit(c,w,(uint32_t)amount>=600u?11u:10u);
    case 88:{
        cycle_counter(w,4,0);
        if(w->action_state==5){
            const uint8_t first=c->walkers[w->target_walker_id].type;
            if(first==23){value=13;*c->shared_result=value;Group(w)=13;break;}
            const uint8_t second=c->walkers[w->target_id_084].type;
            if(second==23 || first==22 || second==22){value=13;*c->shared_result=value;Group(w)=13;break;}
            value=Counter(w)+14u;*c->shared_result=value;break;
        }
        amount=Amount(w);
        if((uint32_t)amount>=500u){value=12;*c->shared_result=value;Group(w)=13;break;}
        if(amount<100)return commit(c,w,10);
        value=11;*c->shared_result=value;Group(w)=13;break;
    }
    case 7:{
        int32_t percentage;
        const PharaohWalkerPhraseStatus status=employment_percent(player(c),&percentage);
        if(status!=PHARAOH_WALKER_PHRASE_VALUE)return status;
        if(percentage<50){value=10;Group(w)=12;*c->shared_result=value;break;}
        amount=Amount(w);
        if(amount>=7){value=11;Group(w)=12;*c->shared_result=value;break;}
        if(amount>=1){value=12;*c->shared_result=value;Group(w)=12;break;}
        value=*c->shared_result;Group(w)=12;break;
    }
    case 89:
        if(player(c)->native_ea6090!=0)return commit(c,w,11);
        amount=Amount(w);
        if((uint32_t)amount>=500u){value=14;*c->shared_result=value;Group(w)=13;break;}
        if(amount>=100){value=13;*c->shared_result=value;Group(w)=13;break;}
        return commit(c,w,12);
    case 78:{
        const volatile PharaohWalkerPhrasePlayer *const p=player(c);
        value=p->native_ea5ea4;
        if(value!=0){
            if(w->action_state==10){value=0;Group(w)=3;*c->shared_result=value;break;}
            if(w->action_state==12){value=1;Group(w)=3;*c->shared_result=value;break;}
        }
        if(p->native_ea6210!=0){
            state=w->action_state;
            if(state!=10 && state!=12){value=2;Group(w)=3;*c->shared_result=value;break;}
        }
        value=current!=0?3u:4u;Group(w)=3;*c->shared_result=value;break;
    }
    case 77:{
        const volatile PharaohWalkerPhrasePlayer *const p=player(c);
        value=p->native_ea5ea4;
        if(value!=0 && w->action_state==10){value=0;Group(w)=3;*c->shared_result=value;break;}
        if(p->native_ea6210!=0){value=1;Group(w)=3;*c->shared_result=value;break;}
        value=current!=0?2u:3u;Group(w)=3;*c->shared_result=value;break;
    }
    case 42:
        value=player(c)->native_ea5ea4;
        if(value==0){Group(w)=2;return commit(c,w,value);}
        if(current!=0){Group(w)=2;return commit(c,w,1);}
        if((int32_t)value<=10){Group(w)=2;return commit(c,w,2);}
        if((int32_t)value<=30){Group(w)=2;return commit(c,w,3);}
        goto existing;
    case 15:case 16:case 17:case 18:case 29:case 30:case 31:case 40:case 87:case 105:
        goto existing;
    default:
        Message(w)=255;*c->shared_result=UINT32_MAX;return PHARAOH_WALKER_PHRASE_VALUE;
    }
    return finish(c,w,value);
existing:
    return finish(c,w,*c->shared_result);
}
