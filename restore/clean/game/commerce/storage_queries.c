#include "pharaoh/commerce/storage_queries.h"

int32_t pharaoh_storage_image_level(int32_t amount)
{
    if (amount == 400) return 3;
    if (amount >= 300 && amount < 400) return 2;
    if (amount >= 200 && amount < 300) return 1;
    return 0;
}

int32_t pharaoh_storageyard_resource_amount(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id, int32_t resource)
{
    int32_t slot;
    int32_t total = 0;
    for (slot = 0; slot < 8; ++slot) {
        building_id = buildings[building_id].next_storage_building_id;
        if (building_id < 1) return 0;
        if (buildings[building_id].subtype != 0 &&
            buildings[building_id].subtype == resource) {
            total += buildings[building_id].loads_stored_render;
        }
    }
    return total;
}
