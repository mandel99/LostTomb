#ifndef PHARAOH_CITY_FINANCE_MONEY_H
#define PHARAOH_CITY_FINANCE_MONEY_H

#include <stdint.h>

#include "pharaoh/building/type_queries.h"

enum {
    PHARAOH_FINANCE_SHARE_ROWS = 3,
    PHARAOH_FINANCE_SHARE_BUILDING_TYPES = 5
};

typedef struct PharaohFinanceMoneyShareRow {
    int32_t building_type_count;
    int32_t building_types[PHARAOH_FINANCE_SHARE_BUILDING_TYPES];
    int32_t weight;
} PharaohFinanceMoneyShareRow;

_Static_assert(sizeof(PharaohFinanceMoneyShareRow) == 0x1c,
    "native finance share row");

int32_t pharaoh_city_finance_money_share(int32_t money,uint32_t index,
    const PharaohFinanceMoneyShareRow table[PHARAOH_FINANCE_SHARE_ROWS],
    const int32_t *building_count_total);

int32_t pharaoh_city_finance_building_money_value(
    const PharaohBuildingTypeRecord *buildings,int32_t building_id,
    int32_t personal_savings,int32_t treasury,
    const PharaohFinanceMoneyShareRow mansion_shares[PHARAOH_FINANCE_SHARE_ROWS],
    const PharaohFinanceMoneyShareRow palace_shares[PHARAOH_FINANCE_SHARE_ROWS],
    const int32_t *building_count_total);

#endif
