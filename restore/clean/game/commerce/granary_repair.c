#include "pharaoh/commerce/granary_repair.h"

static void round_storage_amount(int16_t *amount)
{
    if (*amount < 801) *amount = 800;
    else if (*amount < 1601) *amount = 1600;
    else if (*amount < 2401) *amount = 2400;
    else if (*amount < 3201) *amount = 3200;
}

/* Retained logical-buffer helper; native004D5AD0 uses the record owner below. */
void pharaoh_repair_granary_stock(PharaohGranaryRepairBuilding *buildings,
    size_t building_count, PharaohGranaryRepairStorage *storage_records)
{
    size_t building_id;
    for (building_id = 0; building_id < building_count; ++building_id) {
        PharaohGranaryRepairBuilding *building = &buildings[building_id];
        PharaohGranaryRepairStorage *storage;
        int32_t resource;
        int32_t sum = 0;
        int32_t unused;
        if (building->state != 1u || building->type != PHARAOH_GRANARY_TYPE)
            continue;
        for (resource = 1; resource < PHARAOH_GRANARY_RESOURCE_COUNT;
             ++resource) sum += building->resource_stored[resource];
        unused = PHARAOH_GRANARY_CAPACITY - sum;
        building->resource_stored[0] = (int16_t)(unused < 0 ? 0 : unused);
        storage = &storage_records[building->storage_id];
        for (resource = 1; resource < PHARAOH_GRANARY_RESOURCE_COUNT;
             ++resource) {
            round_storage_amount(&storage->accept_amount[resource]);
            round_storage_amount(&storage->get_amount[resource]);
        }
    }
}

/* Native004D5AD0 with actual264-byte building/196-byte storage records. */
void pharaoh_repair_granary_records(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohBuildingStorageRecord *storages)
{
    for (int32_t id = 0; id < PHARAOH_BUILDING_RECORD_COUNT; ++id) {
        PharaohBuildingTypeRecord *b = &buildings[id];
        if (b->state != 1 || b->building_type != 0x47) continue;
        int32_t sum = 0;
        for (int32_t resource = 1; resource < 36; ++resource)
            sum += b->granary_resource_stored[resource];
        const int32_t unused = 3200 - sum;
        b->granary_resource_stored[0] = (int16_t)(uint16_t)(unused < 0 ? 0 : unused);
        PharaohBuildingStorageRecord *s = &storages[b->storage_id];
        for (int32_t resource = 1; resource < 36; ++resource) {
            round_storage_amount(&s->accept_amount[resource]);
            round_storage_amount(&s->get_amount[resource]);
        }
    }
}

int32_t pharaoh_granary_add_resource(
    PharaohGranaryRepairBuilding *buildings,
    const PharaohGranaryRepairStorage *storage_records,
    int32_t building_id, int32_t resource, int32_t produced, int32_t amount,
    int32_t *food_produced_this_month)
{
    PharaohGranaryRepairBuilding *building;
    const PharaohGranaryRepairStorage *storage;
    int32_t free_capacity;
    uint8_t policy;

    if (building_id < 1) return 1;
    building = &buildings[building_id];
    if ((building->type != 0x47 && building->type != 0xbf) ||
        resource < 1 || resource >= 9) return 0;
    free_capacity = building->resource_stored[0];
    if (free_capacity <= 0) return 0;
    storage = &storage_records[building->storage_id];
    policy = storage->resource_state[resource];
    if (policy == 0 &&
        building->resource_stored[resource] >= storage->accept_amount[resource])
        return 0;
    if (policy == 2 &&
        building->resource_stored[resource] >= storage->get_amount[resource])
        return 0;
    if (produced != 0)
        *food_produced_this_month = (int32_t)
            ((uint32_t)*food_produced_this_month + (uint32_t)amount);
    if (amount < free_capacity) {
        building->resource_stored[resource] = (int16_t)(uint16_t)
            ((uint16_t)building->resource_stored[resource] + (uint32_t)amount);
        building->resource_stored[0] = (int16_t)(uint16_t)
            ((uint16_t)building->resource_stored[0] - (uint32_t)amount);
        return amount;
    }
    building->resource_stored[resource] = (int16_t)(uint16_t)
        ((uint16_t)building->resource_stored[resource] + (uint32_t)free_capacity);
    free_capacity = building->resource_stored[0];
    building->resource_stored[0] = 0;
    return free_capacity;
}
