#include "pharaoh/commerce/luxury_goods.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohLuxuryTradeCity) == 106,
    "native empire city record stride");
_Static_assert(offsetof(PharaohLuxuryTradeCity, culture_id) == 3,
    "native empire city culture field");
_Static_assert(offsetof(PharaohLuxuryTradeCity, trading) == 5,
    "native empire city trading field");
_Static_assert(offsetof(PharaohLuxuryTradeCity, sells_luxury_goods) == 61,
    "native empire city luxury field");

int32_t pharaoh_empire_count_luxury_goods_kinds(
    const PharaohLuxuryGoodsContext *context,
    int32_t counts[PHARAOH_LUXURY_KIND_COUNT])
{
    int32_t total = 0;
    size_t city_id;

    for (city_id = 1; city_id < context->city_count; ++city_id) {
        const PharaohLuxuryTradeCity *city = &context->cities[city_id];
        int32_t resource;

        if (city->in_use == 0 || city->trading == 0)
            continue;
        if (*context->trade_mode != 1 && *context->trade_mode != 3)
            continue;
        if (city->sells_luxury_goods == 0 ||
            context->city_has_trade_limit(context->user_data,
                (int32_t)city_id, 19) == 0)
            continue;

        resource = context->luxury_resource_by_culture[city->culture_id];
        ++counts[resource - PHARAOH_LUXURY_RESOURCE_FIRST];
    }

    for (city_id = 0; city_id < PHARAOH_LUXURY_KIND_COUNT; ++city_id) {
        if (counts[city_id] != 0)
            ++total;
    }
    return total;
}
