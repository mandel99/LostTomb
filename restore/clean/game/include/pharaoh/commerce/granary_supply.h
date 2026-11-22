#ifndef PHARAOH_COMMERCE_GRANARY_SUPPLY_H
#define PHARAOH_COMMERCE_GRANARY_SUPPLY_H

#include "pharaoh/commerce/storage_queries.h"

typedef union PharaohGranarySupplyCandidates {
    uint32_t words[50];
    uint16_t building_ids[100];
} PharaohGranarySupplyCandidates;

typedef struct PharaohGranarySupplyContext {
    const PharaohBuildingTypeRecord *buildings;
    const PharaohBuildingStorageRecord *storages;
    const int32_t *highest_building;
    uint32_t *resource_totals; /* 36 DWORDs */
    PharaohGranarySupplyCandidates *candidates;
    uint32_t *candidate_count;
} PharaohGranarySupplyContext;

void pharaoh_granary_supply_calculate(const PharaohGranarySupplyContext *c);

#endif
