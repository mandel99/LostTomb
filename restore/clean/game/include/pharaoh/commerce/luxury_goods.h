#ifndef PHARAOH_COMMERCE_LUXURY_GOODS_H
#define PHARAOH_COMMERCE_LUXURY_GOODS_H

#include <stddef.h>
#include <stdint.h>

enum {
    PHARAOH_LUXURY_KIND_COUNT = 16,
    PHARAOH_LUXURY_RESOURCE_FIRST = 38
};

typedef struct PharaohLuxuryTradeCity {
    uint8_t in_use;
    uint8_t reserved_001[2];
    uint8_t culture_id;
    uint8_t reserved_004;
    uint8_t trading;
    uint8_t reserved_006[55];
    uint8_t sells_luxury_goods;
    uint8_t reserved_03e[44];
} PharaohLuxuryTradeCity;

typedef struct PharaohLuxuryGoodsContext {
    const PharaohLuxuryTradeCity *cities;
    size_t city_count;
    const int16_t *trade_mode;
    const int32_t *luxury_resource_by_culture;
    uint8_t (*city_has_trade_limit)(void *user_data, int32_t city_id,
        int32_t resource);
    void *user_data;
} PharaohLuxuryGoodsContext;

/* Complete typed C translation of native 0x00559FC0. Counts accumulate. */
int32_t pharaoh_empire_count_luxury_goods_kinds(
    const PharaohLuxuryGoodsContext *context,
    int32_t counts[PHARAOH_LUXURY_KIND_COUNT]);

#endif
