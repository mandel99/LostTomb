#include "pharaoh/scenario/request_travel.h"
#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohScenarioEvent)==124 &&
    offsetof(PharaohScenarioEvent,type)==2 &&
    offsetof(PharaohScenarioEvent,route_runtime_value)==28 &&
    sizeof(PharaohEmpireRouteRecord)==324 &&
    offsetof(PharaohEmpireRouteRecord,point_count)==321 &&
    offsetof(PharaohEmpireRouteRecord,in_use)==322,
    "native request travel fields");

int32_t pharaoh_request_travel_months(const PharaohRequestTravelContext *c,
    const PharaohScenarioEvent *event)
{
    int32_t index;
    if(event->type==2){
        int16_t route_at_58;
        memcpy(&route_at_58,(const uint8_t *)event+0x58,sizeof(route_at_58));
        index=route_at_58;
        if(index<0 || index>=50) return 0;
        PharaohEmpireRouteRecord *const route=&c->routes[index];
        if(route->point_count<=1) route->in_use=0;
        if(route->in_use==0) return 0;
    }else{
        if(!pharaoh_event_uses_trade_city(event)) return 0;
        index=event->route_runtime_value;
        const int32_t city=pharaoh_empire_city_for_trade_route(c->cities,
            (uint32_t)index);
        uint8_t ours_first;
        (void)pharaoh_empire_route_find_for_city(c->objects,c->cities,c->routes,
            city,&ours_first);
    }
    return pharaoh_empire_route_travel_months(&c->routes[index]);
}
