#include "pharaoh/commerce/market_selection.h"

static bool valid_slot(const PharaohMarket *market, int32_t slot)
{
    return slot >= 0 && (size_t)slot < market->inventory_count;
}

bool pharaoh_market_update_image(const PharaohBazaarImage *bazaar,
    const int8_t *desirability, int8_t fancy_threshold,
    int32_t normal_image, int32_t fancy_image,
    void (*place_tiles)(void *, const PharaohBazaarImage *, int32_t),
    void *user_data)
{
    bool fancy;
    if (!bazaar->in_use) return false;
    fancy = desirability[bazaar->grid_offset] > fancy_threshold;
    place_tiles(user_data, bazaar, fancy ? fancy_image : normal_image);
    return fancy;
}

bool pharaoh_market_list_contains(int32_t item, const int32_t *list,
    size_t item_count)
{
    size_t i;
    for (i = 0; i < item_count; ++i) if (list[i] == item) return true;
    return false;
}

static void clear_items(const PharaohMarketRules *rules, int32_t *items)
{
    size_t i;
    for (i = 0; i < rules->item_count; ++i) items[i] = -1;
}

void pharaoh_market_food_items(PharaohMarket *market,
    const PharaohMarketRules *rules, const int32_t *foods, size_t food_count,
    const int32_t *counts, int32_t *items)
{
    size_t pass, i, n = 0;
    clear_items(rules, items);
    for (pass = 0; pass < 2; ++pass) {
        for (i = 0; i < food_count; ++i) {
            int32_t slot;
            bool wanted;
            if (foods[i] == rules->no_resource) continue;
            slot = rules->resource_slot(rules->user_data, foods[i]);
            if (!valid_slot(market, slot)) continue;
            wanted = counts[slot] != 0 && (pass == 0
                ? market->inventory[slot] == 0
                : market->inventory[slot] < rules->food_low &&
                  !pharaoh_market_list_contains(slot, items, rules->item_count));
            if (wanted) {
                items[n++] = slot;
                if (n >= rules->item_count) return;
            }
        }
    }
}

int32_t pharaoh_market_food_destination(PharaohMarket *market,
    const PharaohMarketRules *rules, const int32_t *foods, size_t food_count,
    const int32_t *counts, const int32_t *best)
{
    int16_t lowest = rules->food_very_low;
    int32_t chosen = rules->no_slot;
    size_t i;
    for (i = 0; i < food_count; ++i) {
        int32_t slot;
        if (foods[i] == rules->no_resource) continue;
        slot = rules->resource_slot(rules->user_data, foods[i]);
        if (valid_slot(market, slot) && market->inventory[slot] == 0 && counts[slot] != 0)
            return best[slot];
    }
    for (i = 0; i < food_count; ++i) {
        int32_t slot;
        if (foods[i] == rules->no_resource) continue;
        slot = rules->resource_slot(rules->user_data, foods[i]);
        if (valid_slot(market, slot) && counts[slot] != 0 && market->inventory[slot] < lowest) {
            lowest = market->inventory[slot]; chosen = slot;
        }
    }
    if (chosen == rules->no_slot) {
        for (i = 0; i < food_count; ++i) {
            int32_t slot;
            if (foods[i] == rules->no_resource) continue;
            slot = rules->resource_slot(rules->user_data, foods[i]);
            if (valid_slot(market, slot) && counts[slot] != 0 && market->inventory[slot] < rules->food_low) chosen = slot;
        }
    }
    if (valid_slot(market, chosen)) {
        market->fetch_inventory_id = (uint8_t)chosen;
        return best[chosen];
    }
    return 0;
}

void pharaoh_market_goods_items(PharaohMarket *market,
    const PharaohMarketRules *rules, const int32_t *counts, int32_t *items)
{
    int32_t slot;
    size_t n = 0;
    clear_items(rules, items);
    for (slot = rules->beer_slot; slot < rules->goods_end_slot; ++slot) {
        if (valid_slot(market, slot) && market->inventory[slot] == 0 && counts[slot] != 0) {
            items[n++] = slot;
            if (slot == rules->jewelry_slot && n >= rules->item_count) return;
        }
    }
    for (slot = rules->beer_slot; slot < rules->goods_end_slot; ++slot) {
        if (valid_slot(market, slot) && counts[slot] != 0 && market->inventory[slot] < rules->goods_low &&
            !pharaoh_market_list_contains(slot, items, rules->item_count)) {
            items[n++] = slot;
            if (slot != rules->jewelry_slot && n >= rules->item_count) return;
        }
    }
}

int32_t pharaoh_market_goods_destination(PharaohMarket *market,
    const PharaohMarketRules *rules, const int32_t *counts, const int32_t *best)
{
    int32_t slot, chosen = 0;
    int16_t lowest = rules->goods_low;
    for (slot = rules->beer_slot; slot < rules->goods_end_slot; ++slot) {
        if (valid_slot(market, slot) && market->inventory[slot] == 0 && counts[slot] != 0) {
            market->fetch_inventory_id = (uint8_t)slot; return best[slot];
        }
    }
    for (slot = rules->beer_slot; slot < rules->goods_end_slot; ++slot) {
        if (valid_slot(market, slot) && counts[slot] != 0 && market->inventory[slot] < lowest) {
            chosen = slot; lowest = market->inventory[slot];
        }
    }
    if (chosen == 0) return 0;
    market->fetch_inventory_id = (uint8_t)chosen;
    return best[chosen];
}
