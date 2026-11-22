#include "pharaoh/commerce/resource_supply.h"

#include <string.h>

static uint32_t can_import(const PharaohEmpireSupplyContext *c, int32_t resource);
static uint32_t can_import_potentially(const PharaohEmpireSupplyContext *c,
    int32_t resource);

/* Native00559480: all ten known direct call sites consume TEST AL,AL.
 * Preserve the logical byte, rather than legacy incidental upper EAX. */
uint8_t pharaoh_city_resource_available_from_events(
    const PharaohResourceAvailabilityContext *c, int32_t resource)
{
    for (uint32_t i = *c->event_count; i != 0;) {
        const PharaohScenarioEvent *e = &c->events[--i];
        if (e->type == 23 && (e->item_value == resource ||
            e->item_min == resource || e->item_max == resource)) return 1;
    }
    const PharaohEmpireSupplyContext *s = c->supply;
    if (resource == 12) {
        if ((pharaoh_empire_can_produce_resource(s, 11) ||
             pharaoh_empire_can_produce_resource_potentially(s, 11)) &&
            pharaoh_city_resource_available_from_events(c, 9)) return 1;
        if (pharaoh_city_resource_available_from_events(c, 11) &&
            pharaoh_empire_can_produce_resource(s, 1)) return 1;
        if (can_import_potentially(s, 11) &&
            pharaoh_city_resource_available_from_events(c, 9)) return 1;
        if (pharaoh_city_resource_available_from_events(c, 11) &&
            can_import_potentially(s, 9)) return 1;
        if (!pharaoh_city_resource_available_from_events(c, 11)) return 0;
        return pharaoh_city_resource_available_from_events(c, 9);
    }
    if (resource == 34) {
        if (!pharaoh_empire_can_produce_resource(s, 11) &&
            !pharaoh_empire_can_produce_resource_potentially(s, 11) &&
            !pharaoh_city_resource_available_from_events(c, 11) &&
            !pharaoh_city_resource_available_from_events(c, 13)) return 0;
        return can_import_potentially(s, 31) != 0;
    }
    switch (resource) {
    case 10: if (s->allowed_buildings[41] == 0) return 0; resource = 29; break;
    case 13: resource = 11; break;
    case 15: resource = 14; break;
    case 17: resource = 16; break;
    case 19: resource = 18; break;
    case 23: resource = 22; break;
    case 28: if (s->allowed_buildings[42] == 0) return 0; resource = 20; break;
    case 33: resource = 32; break;
    default: return 0;
    }
    return pharaoh_city_resource_available_from_events(c, resource);
}

/* Complete ResourceAvailability list rebuild, native005596D0. Known flags
 * persist; only the two output counts/lists are cleared before rebuilding. */
void pharaoh_city_resource_determine_available(
    const PharaohResourceAvailabilityContext *c)
{
    const PharaohEmpireSupplyContext *s = c->supply;
    *c->resource_count = 0;
    memset(c->resources, 0, 36 * sizeof(*c->resources));
    memset(c->foods, 0, 36 * sizeof(*c->foods));
    *c->food_count = 0;
    for (int32_t resource = 1; resource < 36; ++resource) {
        if (pharaoh_empire_can_produce_resource(s, resource) ||
            can_import_potentially(s, resource) ||
            pharaoh_city_resource_available_from_events(c, resource) ||
            s->players[*s->current_player].resource_known[resource] != 0) {
            c->resources[*c->resource_count] = resource;
            ++*c->resource_count;
        }
    }
    for (int32_t resource = 1; resource < 9; ++resource) {
        if (pharaoh_empire_can_produce_resource(s, resource) ||
            can_import_potentially(s, resource) ||
            pharaoh_city_resource_available_from_events(c, resource) ||
            s->players[*s->current_player].food_known[resource] != 0) {
            c->foods[*c->food_count] = resource;
            ++*c->food_count;
        }
    }
    PharaohScenarioPlayerRuntime *p = &s->players[*s->current_player];
    for (int32_t i = 0; i < *c->resource_count; ++i)
        if (c->resources[i] != 0) p->resource_known[c->resources[i]] = 1;
    for (int32_t i = 0; i < *c->food_count; ++i)
        if (c->foods[i] != 0) p->food_known[c->foods[i]] = 1;
}

_Static_assert(offsetof(PharaohScenarioEvent, type) == 2 &&
    offsetof(PharaohScenarioEvent, item_value) == 6 &&
    offsetof(PharaohScenarioEvent, item_min) == 8 &&
    offsetof(PharaohScenarioEvent, item_max) == 10,
    "native event resource fields");

static uint32_t can_import(const PharaohEmpireSupplyContext *c, int32_t resource)
{
    const PharaohScenarioPlayerRuntime *p = &c->players[*c->current_player];
    return pharaoh_empire_can_import_resource(c->cities, c->route_limits,
        resource, p->ra_trade_bonus_months, p->ra_trade_penalty_months,
        p->ra_trade_minor_penalty_months);
}

static uint32_t can_import_potentially(const PharaohEmpireSupplyContext *c,
    int32_t resource)
{
    const PharaohScenarioPlayerRuntime *p = &c->players[*c->current_player];
    return pharaoh_empire_can_import_resource_potentially(c->cities, c->route_limits,
        resource, p->ra_trade_bonus_months, p->ra_trade_penalty_months,
        p->ra_trade_minor_penalty_months);
}

/* Completed EconomySupply53 algorithms; native00443B90/00443D90.
 * Recipe remapping, recursive call order and final own-city scans retained. */
uint32_t pharaoh_empire_can_produce_resource(const PharaohEmpireSupplyContext *c, int32_t resource)
{
    if(resource==15){resource=14;if(can_import_potentially(c, 14))return 1;}
    else if(resource==17){resource=16;if(can_import_potentially(c, 16))return 1;}
    else if(resource==33){resource=32;if(can_import_potentially(c, 32))return 1;}
    else if(resource==34){
        if((pharaoh_empire_can_produce_resource(c, 13)||pharaoh_empire_can_produce_resource_potentially(c, 13))&&can_import(c, 31))return 1;
        if(can_import_potentially(c, 13)&&can_import(c, 31))return 1;
    }
    else if(resource==19){resource=18;if(can_import_potentially(c, 18))return 1;}
    else if(resource==13){resource=11;if(can_import_potentially(c, 11))return 1;}
    else if(resource==10){
        resource=29;if(!c->allowed_buildings[41])return 0;
        if(can_import_potentially(c, 29))return 1;
    }
    else if(resource==28){
        resource=20;if(!c->allowed_buildings[42])return 0;
        if(can_import_potentially(c, 20))return 1;
    }
    else if(resource==12){
        if((pharaoh_empire_can_produce_resource(c, 11)||pharaoh_empire_can_produce_resource_potentially(c, 11))&&pharaoh_empire_can_produce_resource(c, 1))return 1;
        if((pharaoh_empire_can_produce_resource(c, 11)||pharaoh_empire_can_produce_resource_potentially(c, 11))&&can_import_potentially(c, 9))return 1;
        if(pharaoh_empire_can_produce_resource(c, 1)&&can_import_potentially(c, 11))return 1;
        if(can_import_potentially(c, 9)&&can_import_potentially(c, 11))return 1;
    }
    else if(resource==23){resource=22;if(can_import_potentially(c, 22))return 1;}
    else if(resource==9){if(pharaoh_empire_can_produce_resource(c, 1))return 1;}
    for(int32_t i=0;i<PHARAOH_EMPIRE_CITY_COUNT;++i){
        const PharaohEmpireCityRecord *city=&c->cities[i];
        if(city->in_use&&city->kind==0&&city->sells[resource]==1)return 1;
    }
    return 0;
}

uint32_t pharaoh_empire_can_produce_resource_potentially(const PharaohEmpireSupplyContext *c, int32_t resource)
{
    if(resource==15){resource=14;if(can_import_potentially(c, 14))return 1;}
    else if(resource==17){resource=16;if(can_import_potentially(c, 16))return 1;}
    else if(resource==33){resource=32;if(can_import_potentially(c, 32))return 1;}
    else if(resource==19){resource=18;if(can_import_potentially(c, 18))return 1;}
    else if(resource==13){resource=11;if(can_import_potentially(c, 11))return 1;}
    else if(resource==10){resource=29;if(can_import_potentially(c, 29))return 1;}
    else if(resource==28){resource=20;if(can_import_potentially(c, 20))return 1;}
    else if(resource==12){
        if((pharaoh_empire_can_produce_resource(c, 1)||pharaoh_empire_can_produce_resource_potentially(c, 9))&&
           (pharaoh_empire_can_produce_resource(c, 11)||pharaoh_empire_can_produce_resource_potentially(c, 11)))return 1;
    }
    else if(resource==34){if(pharaoh_empire_can_produce_resource(c, 13)&&can_import_potentially(c, 31))return 1;}
    else{
        if(resource==23)resource=22;
        if(can_import_potentially(c, resource))return 1;
    }
    for(int32_t i=0;i<PHARAOH_EMPIRE_CITY_COUNT;++i){
        const PharaohEmpireCityRecord *city=&c->cities[i];
        if(city->in_use&&city->kind==0&&city->sells[resource]==1)return 1;
    }
    return 0;
}
