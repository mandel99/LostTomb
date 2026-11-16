#ifndef PHARAOH_COMMERCE_MARKET_SELECTION_H
#define PHARAOH_COMMERCE_MARKET_SELECTION_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct PharaohMarket {
    int16_t *inventory;
    size_t inventory_count;
    uint8_t fetch_inventory_id;
} PharaohMarket;

typedef struct PharaohMarketRules {
    int32_t no_resource;
    int32_t no_slot;
    int32_t beer_slot;
    int32_t goods_end_slot;
    int32_t jewelry_slot;
    int16_t food_low;
    int16_t food_very_low;
    int16_t goods_low;
    size_t item_count;
    int32_t (*resource_slot)(void *user_data, int32_t resource);
    void *user_data;
} PharaohMarketRules;

typedef struct PharaohBazaarImage {
    bool in_use;
    int32_t id;
    int32_t x;
    int32_t y;
    int32_t size;
    int32_t grid_offset;
} PharaohBazaarImage;

bool pharaoh_market_update_image(const PharaohBazaarImage *bazaar,
    const int8_t *desirability, int8_t fancy_threshold,
    int32_t normal_image, int32_t fancy_image,
    void (*place_tiles)(void *, const PharaohBazaarImage *, int32_t),
    void *user_data);
bool pharaoh_market_list_contains(int32_t item, const int32_t *list,
    size_t item_count);
void pharaoh_market_food_items(PharaohMarket *market,
    const PharaohMarketRules *rules, const int32_t *foods, size_t food_count,
    const int32_t *counts, int32_t *items);
int32_t pharaoh_market_food_destination(PharaohMarket *market,
    const PharaohMarketRules *rules, const int32_t *foods, size_t food_count,
    const int32_t *counts, const int32_t *best);
void pharaoh_market_goods_items(PharaohMarket *market,
    const PharaohMarketRules *rules, const int32_t *counts, int32_t *items);
int32_t pharaoh_market_goods_destination(PharaohMarket *market,
    const PharaohMarketRules *rules, const int32_t *counts,
    const int32_t *best);

#endif
