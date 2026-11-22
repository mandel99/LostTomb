#ifndef PHARAOH_COMMERCE_STORAGE_QUERIES_H
#define PHARAOH_COMMERCE_STORAGE_QUERIES_H

#include <stdint.h>
#include "pharaoh/building/type_queries.h"

enum {
    PHARAOH_STORAGE_RESOURCE_SLOTS = 98,
    PHARAOH_STORAGE_ACCEPT_MODE_STRIDE = 196,
    PHARAOH_BUILDING_STORAGE_COUNT = 200
};

typedef struct PharaohBuildingStorageRecord {
    int32_t reserved_00;
    int16_t building_id;
    int16_t reserved_06;
    uint8_t in_use;
    uint8_t empty_all;
    uint8_t resource_state[36];
    uint8_t reserved_2e[6];
    int16_t accept_amount[36];
    int16_t get_amount[36];
} PharaohBuildingStorageRecord;

_Static_assert(sizeof(PharaohBuildingStorageRecord) == 0xc4,
    "native building-storage record layout");

typedef struct PharaohBazaarStockRecord {
    uint8_t reserved_000[0x56];
    int16_t resource_amounts[30];
    uint8_t reserved_092[PHARAOH_BUILDING_RECORD_SIZE - 0x92];
} PharaohBazaarStockRecord;

typedef struct PharaohGranaryDispatchBuilding {
    uint8_t reserved_000[0x10];
    int16_t building_type;
    uint8_t reserved_012[0x40 - 0x12];
    int16_t num_workers;
    uint8_t reserved_042[0x14];
    int16_t resource_stored[9];
    uint8_t reserved_068[0x59];
    uint8_t storage_id;
    uint8_t reserved_0c2[0x46];
} PharaohGranaryDispatchBuilding;

_Static_assert(sizeof(PharaohGranaryDispatchBuilding) ==
    PHARAOH_BUILDING_RECORD_SIZE, "native granary dispatch building layout");

_Static_assert(sizeof(PharaohBazaarStockRecord) == PHARAOH_BUILDING_RECORD_SIZE,
    "bazaar stock view must match the native building stride");

typedef struct PharaohStorageLimitState {
    int16_t building_type;
    uint8_t owner;
    int32_t *orders_scroll;
    int32_t display_mode;
    int32_t storage_resource_count;
    const int32_t *storage_resources;
    const int32_t *food_resources;
    int32_t row;
    uint32_t action;
    uint8_t *accept_modes;
    int16_t *policy_zero_limits;
    int16_t *policy_other_limits;
    uint8_t *redraw_requested;
} PharaohStorageLimitState;

int32_t pharaoh_storage_count_wanted(int32_t first, int32_t second);
uint32_t pharaoh_buyer_food_loads_granary(
    const PharaohBazaarStockRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t actor, int32_t resource, int32_t bazaar_id, int32_t source_id,
    int32_t committed, int32_t category);
uint32_t pharaoh_buyer_goods_loads_storageyard(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const PharaohBazaarStockRecord bazaars[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t resource, int32_t bazaar_id, int32_t source_id,
    int32_t committed, int32_t category);
uint32_t pharaoh_buyer_food_loads_storageyard(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const PharaohBazaarStockRecord bazaars[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t resource, int32_t bazaar_id, int32_t source_id,
    int32_t committed, int32_t category);
int32_t pharaoh_storage_image_level(int32_t amount);
int32_t pharaoh_storage_orders_visible_rows(int32_t display_mode);
void pharaoh_storage_orders_scroll(uint32_t direction, int32_t display_mode,
    int32_t resource_count, int32_t *scroll, uint8_t *redraw_requested);
void pharaoh_storage_orders_adjust_limit(PharaohStorageLimitState *state);
void pharaoh_storage_choose_wanted(int32_t first_percent,
    int32_t second_percent, int32_t count, uint8_t *first_slot,
    uint8_t *second_slot, uint32_t *action);
int32_t pharaoh_storageyard_resource_amount(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id, int32_t resource);
int32_t pharaoh_storageyard_get_space_info(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id);
int32_t pharaoh_granary_resource_to_send(
    const PharaohGranaryDispatchBuilding buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const PharaohBuildingModelRecord models[PHARAOH_BUILDING_MODEL_RECORD_COUNT],
    const PharaohBuildingStorageRecord storage[PHARAOH_BUILDING_STORAGE_COUNT],
    int32_t building_id, int32_t wanted);
void pharaoh_building_storage_clear_all(
    PharaohBuildingStorageRecord storage[PHARAOH_BUILDING_STORAGE_COUNT]);

#endif
