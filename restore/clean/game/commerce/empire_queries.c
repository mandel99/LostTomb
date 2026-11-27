#include "pharaoh/commerce/empire_queries.h"

#include <stddef.h>
#include "pharaoh/core/static_objects.h"

_Static_assert(sizeof(PharaohEmpireObjectRecord) ==
    PHARAOH_EMPIRE_OBJECT_SIZE, "empire object record size");
_Static_assert(offsetof(PharaohEmpireObjectRecord, city_type) == 24,
    "empire object city type offset");
_Static_assert(offsetof(PharaohEmpireObjectRecord, bought_resources) == 52,
    "empire object resource list offset");
_Static_assert(offsetof(PharaohEmpireObjectRecord, trade_amounts) == 62,
    "empire object trade amount offset");
_Static_assert(sizeof(PharaohEmpireRouteRecord) == PHARAOH_EMPIRE_ROUTE_SIZE,
    "empire route record size");

uint32_t pharaoh_empire_object_city_buys_resource(
    const PharaohEmpireObjectRecord *objects, int32_t object_index,
    uint32_t resource_kind)
{
    const PharaohEmpireObjectRecord *const object = &objects[object_index];
    for (int32_t i = 0; i < PHARAOH_EMPIRE_CITY_RESOURCE_SLOTS; ++i) {
        if (object->bought_resources[i] == resource_kind) return 1;
    }
    return 0;
}

uint32_t pharaoh_empire_route_is_sea(
    const PharaohEmpireRouteRecord *routes, int32_t route_kind)
{
    if (route_kind >= 0 && route_kind < 50)
        return routes[route_kind].kind == 2;
    return 0;
}

uint8_t pharaoh_empire_object_trade_amount(
    const PharaohEmpireObjectRecord *objects, int32_t object_index,
    int32_t resource_kind)
{
    const PharaohEmpireObjectRecord *const object = &objects[object_index];
    const uint8_t amount = object->trade_amounts[resource_kind];
    if (object->kind == 1 && object->city_type != 0 && amount < 41)
        return amount;
    return 0;
}

bool pharaoh_empire_city_has_trade_limit(
    const PharaohEmpireCityRecord *cities,
    const PharaohEmpireTradeAmounts *route_limits,
    int32_t city_id, int32_t resource_kind,
    int16_t ra_bonus, int16_t ra_penalty, int16_t ra_minor_penalty)
{
    const uint8_t route_id = cities[city_id].route_id;
    const int32_t quota = route_limits[route_id].resource[resource_kind];
    return pharaoh_gods_ra_adjust_value(quota,
        ra_bonus, ra_penalty, ra_minor_penalty) > 0;
}

static uint32_t empire_can_trade_resource(
    const PharaohEmpireCityRecord cities[PHARAOH_EMPIRE_CITY_COUNT],
    const PharaohEmpireTradeAmounts *route_limits, int32_t resource_kind,
    int16_t ra_bonus, int16_t ra_penalty, int16_t ra_minor_penalty,
    bool importing, bool require_open)
{
    int32_t city_id;
    for (city_id = 0; city_id < PHARAOH_EMPIRE_CITY_COUNT; ++city_id) {
        const PharaohEmpireCityRecord *city = &cities[city_id];
        if (city->in_use != 0u &&
            (city->kind == 1u || city->kind == 3u || city->kind == 5u) &&
            (!require_open || city->is_open != 0u) &&
            (importing ? city->sells[resource_kind] :
                city->buys[resource_kind]) == 1u &&
            pharaoh_empire_city_has_trade_limit(cities, route_limits,
                city_id, resource_kind, ra_bonus, ra_penalty,
                ra_minor_penalty)) return 1;
    }
    return 0;
}

uint32_t pharaoh_empire_can_import_resource(
    const PharaohEmpireCityRecord cities[PHARAOH_EMPIRE_CITY_COUNT],
    const PharaohEmpireTradeAmounts *route_limits, int32_t resource_kind,
    int16_t ra_bonus, int16_t ra_penalty, int16_t ra_minor_penalty)
{
    return empire_can_trade_resource(cities, route_limits, resource_kind,
        ra_bonus, ra_penalty, ra_minor_penalty, true, true);
}

uint32_t pharaoh_empire_can_import_resource_potentially(
    const PharaohEmpireCityRecord cities[PHARAOH_EMPIRE_CITY_COUNT],
    const PharaohEmpireTradeAmounts *route_limits, int32_t resource_kind,
    int16_t ra_bonus, int16_t ra_penalty, int16_t ra_minor_penalty)
{
    return empire_can_trade_resource(cities, route_limits, resource_kind,
        ra_bonus, ra_penalty, ra_minor_penalty, true, false);
}

int32_t pharaoh_empire_object_city_image(
    const PharaohEmpireObjectRecord *objects, int32_t object_index,
    int32_t scenario_rank,
    const PharaohEmpireCityImageCatalog *catalog)
{
    const PharaohEmpireObjectRecord *const object = &objects[object_index];
    switch (object->city_type) {
    case 0:
        return scenario_rank == 10 ? catalog->rank_ten_city_group :
            catalog->default_city_group;
    case 1:
        return catalog->rank_ten_city_group;
    case 2:
        return catalog->type_two_city_group;
    case 3:
        return catalog->default_city_group + 6;
    case 4:
        return catalog->type_four_city_group + 7;
    case 5:
        return catalog->default_city_group +
            catalog->name_offsets[object->name_id][0];
    case 6:
        return catalog->type_four_city_group +
            catalog->name_offsets[object->name_id][0];
    default:
        return 0;
    }
}

int32_t pharaoh_empire_city_for_trade_route(
    const PharaohEmpireCityRecord *cities, uint32_t route_id)
{
    for (int32_t i = 0; i < PHARAOH_EMPIRE_CITY_COUNT; ++i) {
        if (cities[i].in_use != 0 && cities[i].route_id == route_id) {
            return i;
        }
    }
    return 0;
}

int32_t pharaoh_empire_route_find_for_city(
    const PharaohEmpireObjectRecord *objects,
    const PharaohEmpireCityRecord *cities,
    PharaohEmpireRouteRecord *routes, int32_t city_id,
    uint8_t *ours_first)
{
    int32_t ours = 0;
    while (ours < PHARAOH_EMPIRE_OBJECT_COUNT &&
           (objects[ours].in_use == 0 || objects[ours].kind != 1 ||
            objects[ours].city_type != 0)) {
        ++ours;
    }
    if (ours >= PHARAOH_EMPIRE_OBJECT_COUNT) return -1;

    const int32_t theirs = cities[city_id].map_object;
    for (int32_t i = 0; i < PHARAOH_EMPIRE_ROUTE_COUNT; ++i) {
        PharaohEmpireRouteRecord *const route = &routes[i];
        if (route->point_count < 2) route->in_use = 0;
        if (route->in_use != 0 &&
            (route->first_object == theirs || route->last_object == theirs) &&
            (route->first_object == ours || route->last_object == ours)) {
            *ours_first = route->first_object == ours;
            return i;
        }
    }
    return -1;
}

int32_t pharaoh_empire_route_travel_months(
    const PharaohEmpireRouteRecord *route)
{
    const int32_t distance = (int32_t)((uint32_t)route->path_length *
        (uint32_t)route->step);
    const int32_t months = distance / 400;
    return months < 1 ? 1 : months;
}

uint32_t pharaoh_empire_object_city_sells_resource(
    const PharaohEmpireObjectRecord *objects, int32_t object_index,
    uint32_t resource_kind)
{
    const PharaohEmpireObjectRecord *const object = &objects[object_index];
    for (int32_t i = 0; i < PHARAOH_EMPIRE_CITY_SOLD_RESOURCE_SLOTS; ++i) {
        if (object->sold_resources[i] == resource_kind) return 1;
    }
    return 0;
}

uint32_t pharaoh_empire_our_city_sells_resource(
    const PharaohEmpireObjectRecord *objects, uint32_t resource_kind)
{
    for (int32_t i = 0; i < PHARAOH_EMPIRE_OBJECT_COUNT; ++i) {
        const PharaohEmpireObjectRecord *const object = &objects[i];
        if (object->in_use != 0 && object->kind == 1 &&
            object->city_type == 0 &&
            pharaoh_empire_object_city_sells_resource(
                objects, i, resource_kind) != 0) {
            return 1;
        }
    }
    return 0;
}
