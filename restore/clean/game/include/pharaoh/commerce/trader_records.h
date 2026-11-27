#ifndef PHARAOH_COMMERCE_TRADER_RECORDS_H
#define PHARAOH_COMMERCE_TRADER_RECORDS_H

#include "pharaoh/walker/record_queries.h"

#include <stdint.h>

enum {
    PHARAOH_TRADER_RESOURCE_COUNT = 36,
    PHARAOH_TRADER_RECORD_COUNT = 100
};

typedef struct PharaohTradePrice {
    int32_t buy_price;
    int32_t sell_price;
} PharaohTradePrice;

typedef struct PharaohTraderRecord {
    int32_t bought_count;
    int32_t sold_count;
    uint8_t bought_resources[PHARAOH_TRADER_RESOURCE_COUNT];
    uint8_t sold_resources[PHARAOH_TRADER_RESOURCE_COUNT];
    int32_t bought_value;
    int32_t sold_value;
} PharaohTraderRecord;


void pharaoh_trader_create(
    PharaohTraderRecord records[PHARAOH_TRADER_RECORD_COUNT],
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int32_t walker_id, int32_t *next_index);
#endif
