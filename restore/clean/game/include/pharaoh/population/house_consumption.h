#ifndef PHARAOH_POPULATION_HOUSE_CONSUMPTION_H
#define PHARAOH_POPULATION_HOUSE_CONSUMPTION_H

#include "pharaoh/population/house_economy.h"

typedef struct PharaohHouseConsumptionContext {
    volatile PharaohHouseEconomyRecord *buildings;
    const volatile PharaohHouseModel *models;
    const volatile int32_t *day; /* 78D7C4 */
} PharaohHouseConsumptionContext;

/* 454B40. Actual callers select day 0 or 7. The models may contain signed
 * quantities, including values that wrap when a large house multiplies by four.
 * The observed caller chain supplies a zero callee-saved EBX; no machine-frame
 * input is part of this logical operation. Valid building/model indices and a
 * nonnegative day are required, as established by the native call sites. */
void pharaoh_house_consume_goods(const PharaohHouseConsumptionContext *,
    int32_t building);

#endif
