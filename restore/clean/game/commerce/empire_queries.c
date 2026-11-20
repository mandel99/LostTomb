#include "pharaoh/commerce/empire_queries.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohEmpireObjectRecord) ==
    PHARAOH_EMPIRE_OBJECT_SIZE, "empire object record size");
_Static_assert(offsetof(PharaohEmpireObjectRecord, trade_enabled) == 24,
    "empire object trade flag offset");
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
    if (object->kind == 1 && object->trade_enabled != 0 && amount < 41)
        return amount;
    return 0;
}
