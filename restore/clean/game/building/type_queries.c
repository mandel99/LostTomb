#include <string.h>
#include "pharaoh/building/type_queries.h"

int32_t pharaoh_building_group_index(
    const PharaohBuildingTypeRecord *buildings, int32_t id)
{
    switch (buildings[id].building_type) {
    case 0x3c: case 0x41: return 0;
    case 0x3d: case 0x42: return 1;
    case 0x3e: case 0x43: return 2;
    case 0x3f: case 0x44: return 3;
    case 0x40: case 0x45: return 4;
    default: return -1;
    }
}

void pharaoh_buildings_migrate_record_layout(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT])
{
    int32_t id;
    for (id = 0; id < PHARAOH_BUILDING_RECORD_COUNT; ++id) {
        uint8_t *record = (uint8_t *)&buildings[id];
        uint8_t converted[PHARAOH_BUILDING_RECORD_SIZE];
        int16_t building_type;

        memset(converted, 0, sizeof(converted));
        memcpy(converted, record, 0xf4u);
        memcpy(&building_type, converted + 0x10, sizeof(building_type));
        if (building_type < 10 || building_type > 0x1d) {
            if (building_type == 0xb7 || building_type == 0xd2) {
                memcpy(converted + 0x54, record + 0x54, 0x30u);
            } else {
                memcpy(converted + 0x56, record + 0x56, 0x3cu);
                memset(converted + 0x92, 0, 0x0cu);
                memcpy(converted + 0x9e, record + 0x92, 0x0du);
                memcpy(converted + 0xac, record + 0xa0, 0x0cu);
            }
        } else {
            memcpy(converted + 0x54, record + 0x54, 0x3cu);
        }
        memcpy(converted + 0xb8, record + 0xac, 0x12u);
        memcpy(converted + 0xcc, record + 0xc0, 0x34u);
        memcpy(record, converted, sizeof(converted));
    }
}

void pharaoh_buildings_reset_runtime_fields(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT])
{
    int32_t id;
    for (id = 1; id < PHARAOH_BUILDING_RECORD_COUNT; ++id) {
        PharaohBuildingTypeRecord *building = &buildings[id];
        switch (building->building_type) {
        case 0x88:
            building->runtime_value_ca = building->runtime_value_c1;
            building->runtime_value_c1 = 0;
            building->repair_queue_id = 0;
            break;
        case 0x4a:
            building->repair_queue_id = building->runtime_value_c1;
            building->runtime_value_c1 = 0;
            building->runtime_value_ca = 0;
            break;
        case 0x46:
        case 0x47:
        case 0x48:
        case 0xbe:
        case 0xbf:
        case 0xc1:
            building->repair_queue_id = 0;
            building->runtime_value_ca = 0;
            break;
        default:
            building->runtime_value_c1 = 0;
            building->repair_queue_id = 0;
            building->runtime_value_ca = 0;
            break;
        }
    }
}

void pharaoh_monuments_reset_runtime_fields(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t pyramid_style, const int32_t *pyramid_part_counts,
    const int32_t *mastaba_part_counts, int32_t *sphinx_completed)
{
    int32_t id = 1;
    do {
        PharaohBuildingTypeRecord *building = &buildings[id];
        uint8_t *record = (uint8_t *)building;
        if (building->state == 1u) {
            switch (building->building_type) {
            case 0xb7:
                record[0x80] = 0;
                id += -1 + (pyramid_style == 5 ?
                    mastaba_part_counts[building->pyramid_size_variant] :
                    pyramid_part_counts[building->pyramid_size_variant]);
                break;
            case 0xcf:
            case 0xd0:
                record[0x80] = 0;
                break;
            case 0xd2:
                *sphinx_completed = 0;
                break;
            case 0xd5:
                record[0x68] = 0;
                id += 99;
                break;
            case 0xde:
                record[0x74] = 0;
                id += 0x73;
                break;
            case 0xe1:
                record[0x68] = 0;
                id += 0x18;
                break;
            case 0xe3:
                record[0x68] = 0;
                id += 8;
                break;
            case 0xe4:
                record[0x68] = 0;
                break;
            case 0xe5:
                record[0x9d] = 0;
                id += 0xa6;
                break;
            case 0xe6:
                record[0x60] = 0;
                id += 0x0f;
                break;
            case 0xea:
                record[0x9d] = 0;
                id += 0x9d;
                break;
            case 0xeb:
                record[0x9d] = 0;
                id += 0x1b4;
                break;
            case 0xec:
                record[0x9d] = 0;
                id += 0x200;
                break;
            default:
                break;
            }
        }
        ++id;
    } while (id < PHARAOH_BUILDING_RECORD_COUNT);
}

/* Native0x0046F190; complete C required by scene initialization. */
int32_t pharaoh_building_count_active_type_b6(
    const PharaohBuildingTypeRecord *buildings, int32_t highest_id)
{
    int32_t id, count = 0;
    for (id = 1; id <= highest_id; ++id)
        if (buildings[id].state == 1 && buildings[id].building_type == 0xb6) ++count;
    return count;
}

/* Native0x004C52D0; complete C required by scene initialization. */
bool pharaoh_temple_upgrade_present(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    uint32_t upgrade)
{
    const uint8_t current = buildings[building_id].temple_upgrade;
    return current == 3u || current == upgrade;
}

/* Completed prior native00541AB0 leaf. */
int32_t pharaoh_sun_temple_part_size(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    const PharaohSunTemplePartDefinition *part_definitions)
{
    return part_definitions[buildings[building_id].subtype].size;
}

void pharaoh_building_repair_queue_add(
    PharaohBuildingTypeRecord *buildings, int32_t building_id,
    PharaohBuildingRepairQueueEntry repair_queue[10],
    PharaohBuildingRepairDebugOutput output_debug, void *user_data)
{
    int32_t index;
    for (index = 0; index < 10; ++index) {
        if (repair_queue[index].active == 0u) {
            buildings[building_id].repair_queue_id = (uint8_t)(index + 1);
            repair_queue[index].active = 1;
            repair_queue[index].building_id = building_id;
            return;
        }
    }
    if (output_debug != 0) {
        output_debug(user_data,
            "unable to find a repair queue for this shipyard!\n");
    }
}

void pharaoh_building_repair_queue_remove(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    PharaohBuildingRepairQueueEntry repair_queue[10])
{
    const int32_t index = (int32_t)buildings[building_id].repair_queue_id - 1;
    if (index >= 0 && index < 10) {
        repair_queue[index].active = 0;
        repair_queue[index].building_id = 0;
    }
}
