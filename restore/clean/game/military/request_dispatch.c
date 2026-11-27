#include "pharaoh/military/request_dispatch.h"
#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohNativeLegionRecord)==144 &&
    offsetof(PharaohNativeLegionRecord,deployment_blocked)==3 &&
    offsetof(PharaohNativeLegionRecord,member_ids)==8 &&
    offsetof(PharaohNativeLegionRecord,member_count)==40 &&
    offsetof(PharaohNativeLegionRecord,order_x)==50 &&
    offsetof(PharaohNativeLegionRecord,order_y)==52 &&
    offsetof(PharaohNativeLegionRecord,kind)==66 &&
    offsetof(PharaohNativeLegionRecord,empire_service)==103 &&
    offsetof(PharaohNativeLegionRecord,deployment_status)==130,
    "native formation dispatch fields");
_Static_assert(sizeof(PharaohWalkerQueryRecord)==388 &&
    offsetof(PharaohWalkerQueryRecord,state)==14 &&
    offsetof(PharaohWalkerQueryRecord,owner)==15 &&
    offsetof(PharaohWalkerQueryRecord,action_state)==56 &&
    offsetof(PharaohWalkerQueryRecord,ship_formation_id_092)==146 &&
    offsetof(PharaohWalkerQueryRecord,request_event_id)==306 &&
    sizeof(PharaohScenarioEvent)==124 &&
    offsetof(PharaohScenarioEvent,route_runtime_value)==28 &&
    offsetof(PharaohEmpireRouteRecord,kind)==320,
    "native walker/event dispatch fields");

void pharaoh_formation_legions_dispatch_to_request(
    const PharaohMilitaryRequestDispatchContext *c,int32_t event_id)
{
    if(event_id<0 || event_id>=*c->event_count) return;
    const int32_t route=c->events[event_id].route_runtime_value;
    if(route<0 || route>=50) return;
    const int naval=c->routes[route].kind==2;
    PharaohLegionRequestContext *const legions=c->legions;
    for(int32_t id=1;id<50;++id){
        PharaohNativeLegionRecord *const f=&legions->formations[id];
        if(f->state!=1 || f->kind==0 || f->empire_service==0 ||
            f->member_count==0 || f->deployment_status>=0) continue;
        const int on_ship=pharaoh_formation_legion_is_on_ship(f->member_ids,
            legions->walkers)!=0;
        if(naval!=on_ship) continue;
        f->deployment_status=(int16_t)event_id;
        f->deployment_blocked=0;
        if(on_ship){
            const int32_t ship=pharaoh_walker_find_legion_transport_ship(
                legions,id);
            /* Parent overwrites the child's2000 miss sentinel with zero. */
            *legions->current_walker_id=ship;
            if(ship!=0) legions->walkers[ship].action_state=17;
        }else{
            f->order_x=*c->city_x;
            f->order_y=*c->city_y;
            for(int32_t i=0;i<f->member_count;++i){
                int16_t walker;
                /* Original BYTE count is not capped at the16 roster slots. */
                memcpy(&walker,(const uint8_t *)legions->formations+
                    (size_t)id*144+8+(size_t)i*2,sizeof(walker));
                if(walker<=0) continue;
                PharaohWalkerQueryRecord *const w=&legions->walkers[walker];
                if(w->state!=1 || w->action_state==4) continue;
                w->action_state=15;
                pharaoh_walker_detach_slot_link(legions->walkers,
                    c->slot_walkers,walker);
            }
        }
    }
    if(naval){
        const uint32_t player=*c->current_player;
        *legions->current_walker_id=2000;
        for(int32_t id=1;id<2000;++id){
            PharaohWalkerQueryRecord *const w=&legions->walkers[id];
            if(w->state!=0 && w->type==78 && (uint32_t)w->owner==player &&
                w->reserved_002[0]!=0 && w->request_event_id<0){
                w->request_event_id=(int16_t)event_id;
                w->action_state=17;
            }
        }
    }
}
