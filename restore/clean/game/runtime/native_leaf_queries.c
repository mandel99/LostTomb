#include "pharaoh/runtime/native_leaf_queries.h"
#include <stddef.h>

_Static_assert(sizeof(void *)==4&&offsetof(PharaohIndexedChainNode,key)==8,"native indexed node");
_Static_assert(sizeof(PharaohPopulationLeafPlayer)==0x49d8,"native census stride");
_Static_assert(offsetof(PharaohPopulationLeafPlayer,people_at_age)==4,"census WORD origin");
_Static_assert(sizeof(PharaohScenarioEvent)==124&&offsetof(PharaohScenarioEvent,type)==2,"event BYTE");
_Static_assert(offsetof(PharaohNativeLegionRecord,deployment_status)==0x82,"formation status WORD");
_Static_assert(offsetof(PharaohWalkerQueryRecord,request_event_id)==0x132,"ship status WORD");

/* Original decompiled void body lost the observed EAX result. Native8
 * instructions and sole consumer prove this pointer-returning algorithm. */
PharaohIndexedChainNode *pharaoh_indexed_chain_find(PharaohIndexedChainNode *node,int32_t minimum)
{
    while(((volatile PharaohIndexedChainNode *)node)->key<minimum)
        node=((volatile PharaohIndexedChainNode *)node)->next;
    return node;
}

/* Native consumer reads AL only; upper EAX contains incidental scaled ids. */
uint8_t pharaoh_walker_group_status_nonnegative(const PharaohWalkerQueryRecord *walkers,
    const PharaohNativeLegionRecord *formations,int32_t id)
{
    const PharaohWalkerQueryRecord *w=&walkers[id];
    switch(w->type){
    case 11:case 12:case 13:return (uint8_t)(formations[w->formation_id_60].deployment_status>=0);
    case 77:case 78:return (uint8_t)(w->request_event_id>=0);
    default:return 0;
    }
}

uint32_t pharaoh_trade_ship_assignment_invalid(const PharaohWalkerQueryRecord *walkers,
    const PharaohBuildingTypeRecord *buildings,int32_t id)
{
    const PharaohBuildingTypeRecord *b=&buildings[walkers[id].entertainment_building_id];
    if(b->state!=1)return 1;
    if(b->building_type!=75&&b->building_type!=192)return 1;
    if(b->num_workers<=0)return 1;
    return b->runtime_value_b4!=id;
}

uint8_t pharaoh_building_overlay_allows(const PharaohBuildingTypeRecord *buildings,
    const volatile uint32_t *current_mode,const int32_t *const *type_lists,int32_t id)
{
    const uint32_t mode=*current_mode;
    if(mode==51)return 1;
    if(mode==8&&*(const volatile int16_t *)((const uint8_t *)&buildings[id]+0x4a)!=0)return 1;
    const int32_t *types=type_lists[mode];
    if(types==NULL)return 0;
    return (uint8_t)pharaoh_building_type_in_list(buildings,id,types);
}

/* Native ECX is the count/table object; physical AL is the sole result used.
 * The old frame adapter exposed incoming EAX solely to preserve unused bits. */
uint8_t pharaoh_event_deferred_allows(const PharaohDeferredEventFilter *c,int32_t index)
{
    const int32_t first=*c->minimum_index;
    if(first<0||first>index)return 0;
    if(index<0||index>=*c->event_count)return 0;
    const volatile PharaohScenarioEvent *event=&c->events[index];
    if(event==NULL)return 0;
    const uint8_t type=event->type,phase=*c->phase;
    return (uint8_t)(type==2?phase!=0:phase==0);
}

/* Whole current/old Unit02042 algorithm. A negative count retains native
 * nonzero/modulo32 behavior; original ten-consecutive-empty exit is kept. */
void pharaoh_population_remove_from_decade(volatile PharaohPopulationLeafPlayer *players,
    int32_t player,int32_t decade,int32_t count)
{
    uint32_t remaining=(uint32_t)count,cursor=0,misses=0;
    const uint32_t start=(uint32_t)decade*10u;
    while(remaining!=0&&misses<10){
        volatile int16_t *age=&players[player].people_at_age[start+cursor];
        const int16_t value=*age;
        if(value>0){*age=(int16_t)(value-1);--remaining;misses=0;}
        else ++misses;
        if(++cursor>=10)cursor=0;
    }
}

/* Full graph-date RAM algorithm from Unit02053. The sole native caller
 * overwrites EAX before use; no incidental arithmetic result is invented. */
void pharaoh_population_graph_dates(const PharaohPopulationGraphContext *c,int32_t distance)
{
    const uint32_t player=*c->current_player;
    const int32_t years=distance/12,remainder=distance%12;
    uint32_t year,month;
    if(c->players[player].recorded_months<=distance){
        year=(uint32_t)(int32_t)*c->scenario_start_year;
        c->dates->start_month=0;c->dates->start_year=(int32_t)year;
        c->dates->end_month=remainder;
        c->dates->end_year=(int32_t)(year+(uint32_t)years);
        return;
    }
    year=(uint32_t)*c->current_year;c->dates->end_year=(int32_t)year;
    month=(uint32_t)*c->current_month-1u;c->dates->end_month=(int32_t)month;
    if((int32_t)month<0){month+=12u;--year;c->dates->end_month=(int32_t)month;c->dates->end_year=(int32_t)year;}
    month-=(uint32_t)remainder;c->dates->start_month=(int32_t)month;
    if((int32_t)month<0){month+=12u;c->dates->start_month=(int32_t)month;}
    /* Native deliberately does not borrow another year for this correction. */
    c->dates->start_year=(int32_t)(year-(uint32_t)years);
}

void pharaoh_pyramid_entry_road_coordinates(const PharaohSpecialEntryMap *map,
    int32_t id,volatile int32_t *x,volatile int32_t *y)
{
    const volatile PharaohBuildingTypeRecord *buildings=map->buildings;
    const uint32_t root=(uint32_t)id-(uint32_t)(int32_t)buildings[id].subtype;
    const int32_t style=*map->pyramid_style;
    const uint8_t variant=buildings[root].pyramid_size_variant;
    uint32_t selected;
    if(style==5)selected=root+(uint32_t)map->mastaba_entries[variant].entry;
    else selected=root+(uint32_t)map->pyramid_entries[variant].entry;
    *x=buildings[selected].road_access_x;
    *y=buildings[selected].road_access_y;
}
