#ifndef PHARAOH_COMMERCE_GRANARY_REPAIR_H
#define PHARAOH_COMMERCE_GRANARY_REPAIR_H

#include <stddef.h>
#include <stdint.h>
#include "pharaoh/commerce/storage_queries.h"

enum {
    PHARAOH_GRANARY_RESOURCE_COUNT = 36,
    PHARAOH_GRANARY_TYPE = 0x47,
    PHARAOH_GRANARY_CAPACITY = 3200
};

typedef struct PharaohGranaryRepairBuilding {
    uint8_t state;
    int16_t type;
    uint8_t storage_id;
    int16_t resource_stored[PHARAOH_GRANARY_RESOURCE_COUNT];
} PharaohGranaryRepairBuilding;

typedef struct PharaohGranaryRepairStorage {
    uint8_t resource_state[PHARAOH_GRANARY_RESOURCE_COUNT];
    int16_t accept_amount[PHARAOH_GRANARY_RESOURCE_COUNT];
    int16_t get_amount[PHARAOH_GRANARY_RESOURCE_COUNT];
} PharaohGranaryRepairStorage;

void pharaoh_repair_granary_stock(PharaohGranaryRepairBuilding *buildings,
    size_t building_count, PharaohGranaryRepairStorage *storage_records);
void pharaoh_repair_granary_records(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohBuildingStorageRecord *storages);
int32_t pharaoh_granary_add_resource(
    PharaohGranaryRepairBuilding *buildings,
    const PharaohGranaryRepairStorage *storage_records,
    int32_t building_id, int32_t resource, int32_t produced, int32_t amount,
    int32_t *food_produced_this_month);

#endif
