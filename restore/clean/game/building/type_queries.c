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

int32_t pharaoh_quarry_image_group(int32_t resource)
{
    return resource == 25 ? 0x1b7 : 0x133;
}

void pharaoh_construction_ferry_cancel(int32_t *construction_state,
    PharaohBuildingTypeRecord *buildings, int32_t building_id)
{
    *construction_state = 0;
    buildings[building_id].state = 6;
}

bool pharaoh_construction_cancel(PharaohConstructionCancelState *state,
    PharaohBuildingTypeRecord *buildings)
{
    const bool changed = state->current_window != 1u ||
        state->editor_construction_tool != 0;

    state->current_window = 1;
    state->editor_construction_tool = 0;
    state->canal_connector_drag_started = 0;
    state->field_6059dc = 0;
    state->field_6059e0 = 0;
    state->monument_selections = 0;
    state->field_6059d8 = 0;
    if (state->ferry_state == 1) {
        pharaoh_construction_ferry_cancel(&state->ferry_state, buildings,
            state->ferry_first_building);
    }
    if (state->ferry_state == 2) {
        state->ferry_state = 0;
    }
    state->redraw_requested = 1;
    return changed;
}

uint32_t pharaoh_building_resource_is_permitted(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    const int16_t *resource_permissions)
{
    /* Native signed-WORD condition(type<106 && type>114) cannot hold;
       BL is zero before the farm query. Both switch destinations then reach
       the same resource flag. Preserve the real child call and flag width. */
    (void)pharaoh_building_is_farm(buildings, building_id);
    return resource_permissions[buildings[building_id].resource_id] != 0;
}

/* Complete004EA9C0. Logical AL predicate; expanded existing TYPE_IN_CASES. */
int32_t pharaoh_building_type_is_004ea9c0_group(int32_t type)
{
    switch(type){
    case 0x52: case 0x53: case 0x54: case 0x66: case 0x67: case 0x68: return 1;
    default: return 0;
    }
}

int32_t pharaoh_building_count_qualifying_004ea280(
    const PharaohBuildingTypeRecord *buildings, int32_t threshold)
{
    int32_t count=0;
    for(int32_t id=1;id<PHARAOH_BUILDING_RECORD_COUNT;++id){
        const PharaohBuildingTypeRecord *b=&buildings[id];
        if(b->state!=0 && b->house_size!=0 && b->house_population!=0 &&
            threshold<=(int32_t)b->building_type-10) ++count;
    }
    return count;
}

void pharaoh_city_victory_update_win_houses(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t threshold, int32_t *win_house_count)
{
    *win_house_count =
        pharaoh_building_count_qualifying_004ea280(buildings, threshold);
}

int32_t pharaoh_building_stores_treasury_funds(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id)
{
    switch (buildings[building_id].building_type) {
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x57:
        case 0xb8:
        case 0xbb:
        case 0xbc:
        case 0xbd:
            return 1;
        default:
            return 0;
    }
}

int32_t pharaoh_city_buildings_has_treasury(
    const PharaohBuildingTypeRecord *buildings, int32_t highest_id)
{
    int32_t id;
    for (id = 1; id <= highest_id; ++id) {
        if (buildings[id].state == 1u &&
            pharaoh_building_stores_treasury_funds(buildings, id) != 0) {
            return 1;
        }
    }
    return 0;
}

#define TYPE_IN_CASES(name, cases) \
int32_t name(int32_t type) { switch (type) { cases return 1; default: return 0; } }
TYPE_IN_CASES(pharaoh_building_type_is_0046fde0_group,
    case 5: case 8: case 9: case 0x5a: case 100: case 101: case 102:
    case 103: case 104: case 105: case 196: case 224:)

int32_t pharaoh_building_main_part_id(
    const PharaohBuildingTypeRecord *buildings, int32_t id)
{
    return pharaoh_building_follow_link(buildings, id);
}

bool pharaoh_farm_has_water_access(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id)
{
    return buildings[building_id].farm_water_access != 0u;
}

int32_t pharaoh_building_type_is_00488080_group(
    const PharaohBuildingTypeRecord *buildings, int32_t id)
{
    switch (buildings[id].building_type) {
    case 100: case 101: case 102: case 103: case 104: case 105:
    case 196: case 224: return 1;
    default: return 0;
    }
}

int32_t pharaoh_building_type_is_00489880_group(
    const PharaohBuildingTypeRecord *buildings, int32_t id)
{
    switch (buildings[id].building_type) {
    case 0x66: case 0x67: case 0x68: case 0x69: case 0xc4: return 1;
    default: return 0;
    }
}

int32_t pharaoh_building_type_is_monument(int32_t type)
{
    switch (type) {
        case 0xb7: case 0xcf: case 0xd0: case 0xd2: case 0xd5:
        case 0xde: case 0xe1: case 0xe3: case 0xe4: case 0xe5:
        case 0xe6: case 0xea: case 0xeb: case 0xec:
            return 1;
        default:
            return 0;
    }
}

int32_t pharaoh_building_is_water_lift_footprint(
    const int16_t *building_ids,
    const PharaohBuildingTypeRecord *buildings,
    const int32_t footprint_offsets[4], int32_t grid_offset)
{
    const int32_t building_id = building_ids[grid_offset];
    int32_t index;
    if (building_id == 0 || buildings[building_id].building_type != 0x5a) return 0;
    for (index = 0; index < 4; ++index)
        if (building_ids[grid_offset + footprint_offsets[index]] != building_id)
            return 0;
    return 1;
}

#define BUILDING_BYTE(building, offset) \
    (*((const uint8_t *)(building) + (offset)))

#define BUILDING_DWORD(building, offset) \
    (*((const int32_t *)((const uint8_t *)(building) + (offset))))

#define DEFINE_SERVICE_NEED(name, excluded, field) \
int32_t name(const PharaohBuildingTypeRecord *buildings, int32_t id, \
    int32_t enabled) \
{ \
    const PharaohBuildingTypeRecord *building = &buildings[id]; \
    if (building->building_type == (excluded) || BUILDING_BYTE(building, 5) == 0 || \
        enabled == 0) return 0; \
    return 10 - BUILDING_BYTE(building, (field)) / 10; \
}

static int32_t market_food_slot(int32_t resource)
{
    switch (resource) {
    case 1: return 0; case 2: return 1; case 3: return 2; case 4: return 3;
    case 5: return 4; case 6: return 5; case 7: return 6; case 8: return 7;
    case 15: return 8; case 13: return 9; case 17: return 10; case 19: return 11;
    default: return -1;
    }
}

int32_t pharaoh_building_service_status_00434060(
    const PharaohBuildingTypeRecord *buildings, int32_t id,
    int32_t first_enabled, int32_t second_enabled)
{
    const PharaohBuildingTypeRecord *b = &buildings[id];
    uint8_t status;
    if (b->building_type == 0x33 || b->building_type == 0x35 || BUILDING_BYTE(b, 5) == 0 ||
        first_enabled + second_enabled == 0) return 0;
    status = BUILDING_BYTE(b, 0x81);
    if (status == 1) return 8;
    if (status == 2) return 5;
    if (status == 3) return 2;
    return 0;
}

int32_t pharaoh_building_service_need_004340e0(
    const PharaohBuildingTypeRecord *buildings, int32_t id,
    int32_t first_enabled, int32_t second_enabled)
{
    const PharaohBuildingTypeRecord *b = &buildings[id];
    if (b->building_type == 0x1f || b->building_type == 0x24 || BUILDING_BYTE(b, 5) == 0 ||
        first_enabled + second_enabled == 0) return 0;
    return 10 - BUILDING_BYTE(b, 0x6c) / 10;
}

int32_t pharaoh_building_service_need_00434150(
    const PharaohBuildingTypeRecord *buildings, int32_t id,
    int32_t first_enabled, int32_t second_enabled, int32_t third_enabled)
{
    const PharaohBuildingTypeRecord *b = &buildings[id];
    if (b->building_type == 0x1e || b->building_type == 0x22 || b->building_type == 0x24 ||
        BUILDING_BYTE(b, 5) == 0 ||
        first_enabled + second_enabled + third_enabled == 0) return 0;
    return 10 - BUILDING_BYTE(b, 0x6d) / 10;
}

int32_t pharaoh_building_service_need_004341d0(
    const PharaohBuildingTypeRecord *buildings, int32_t id,
    int32_t first_enabled, int32_t second_enabled, int32_t third_enabled)
{
    const PharaohBuildingTypeRecord *b = &buildings[id];
    if (b->building_type == 0x21 || b->building_type == 0x22 || b->building_type == 0x23 ||
        BUILDING_BYTE(b, 5) == 0 ||
        first_enabled + second_enabled + third_enabled == 0) return 0;
    return 10 - BUILDING_BYTE(b, 0x6f) / 10;
}

int32_t pharaoh_building_service_need_00434250(
    const PharaohBuildingTypeRecord *buildings, int32_t id,
    int32_t first_enabled, int32_t second_enabled)
{
    const PharaohBuildingTypeRecord *b = &buildings[id];
    if (b->building_type == 0x20 || b->building_type == 0x25 || BUILDING_BYTE(b, 5) == 0 ||
        first_enabled + second_enabled == 0) return 0;
    return 10 - BUILDING_BYTE(b, 0x71) / 10;
}

DEFINE_SERVICE_NEED(pharaoh_building_service_need_004343a0, 0xb4, 0x77)

DEFINE_SERVICE_NEED(pharaoh_building_service_need_00434510, 0x33, 0x72)

DEFINE_SERVICE_NEED(pharaoh_building_service_need_00434570, 0x35, 0x73)

DEFINE_SERVICE_NEED(pharaoh_building_service_need_004345d0, 0x31, 0x75)

DEFINE_SERVICE_NEED(pharaoh_building_service_need_00434630, 0x31, 0x6e)

DEFINE_SERVICE_NEED(pharaoh_building_service_need_00434690, 0x2e, 0x76)

DEFINE_SERVICE_NEED(pharaoh_building_service_need_004346f0, 0x2f, 0x78)

DEFINE_SERVICE_NEED(pharaoh_building_service_need_00434750, 0xb8, 0x70)

int32_t pharaoh_building_type_in_list(
    const PharaohBuildingTypeRecord *buildings, int32_t id,
    const int32_t *types)
{
    while (*types != 0) {
        if (buildings[id].building_type == *types) return 1;
        ++types;
    }
    return 0;
}

void pharaoh_game_tick_update_tomb_material_requests(
    const PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    uint8_t request_flags[4])
{
    static const int16_t building_types[4] = { 0xe5, 0xea, 0xeb, 0xec };
    uint8_t found[4] = { 0, 0, 0, 0 };
    uint8_t previous[4];

    for (int32_t i = 0; i < 4; ++i) previous[i] = request_flags[i] != 0;
    for (int32_t i = 0; i < PHARAOH_WALKER_RECORD_COUNT; ++i) {
        const PharaohWalkerQueryRecord *const walker = &walkers[i];
        if (walker->type != 0x55 ||
            (walker->action_state != 15 && walker->action_state != 16 &&
             walker->action_state != 36)) {
            continue;
        }
        const int16_t references[2] = {
            walker->related_building_id_5c,
            walker->entertainment_building_id
        };
        for (int32_t reference = 0; reference < 2; ++reference) {
            const int16_t type = buildings[references[reference]].building_type;
            for (int32_t material = 0; material < 4; ++material) {
                if (type == building_types[material]) found[material] = 1;
            }
        }
    }
    for (int32_t i = 0; i < 4; ++i)
        if (previous[i] != 0 && found[i] == 0) request_flags[i] = 0;
}

int32_t pharaoh_house_check_desirability(
    const PharaohBuildingTypeRecord *buildings,
    const PharaohHouseModel *house_models, int32_t id)
{
    const uint8_t *b = (const uint8_t *)&buildings[id];
    const int32_t level = *(const int16_t *)(b + 0x12);
    const int32_t desirability = (int8_t)b[0xbe];
    const int32_t evolve = level > 0x12 ? 1000 :
        house_models[level].evolve_desirability;
    if (desirability <= house_models[level].devolve_desirability) return -1;
    return evolve <= desirability;
}

void pharaoh_buildings_decay_houses_covered(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t highest_id)
{
    int32_t id;
    for (id = 1; id <= highest_id; ++id) {
        PharaohBuildingTypeRecord *building = &buildings[id];
        if (building->state == 0u || building->building_type == 59 ||
            building->building_type == 172 || building->building_type == 173)
            continue;
        if (building->houses_covered < 2) building->houses_covered = 0;
        else --building->houses_covered;
    }
}

void pharaoh_bazaar_update_supply(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t highest_id, const PharaohHouseModel *house_models,
    int32_t bazaar_id, int32_t radius)
{
    const int32_t bazaar_x = buildings[bazaar_id].x;
    const int32_t bazaar_y = buildings[bazaar_id].y;
    int32_t id;

    for (id = 1; id <= highest_id; ++id) {
        const PharaohBuildingTypeRecord *house = &buildings[id];
        int32_t dx;
        int32_t dy;
        int32_t level;
        int32_t amount;

        if (house->state != 1u || house->house_size == 0u ||
            house->house_population == 0) continue;
        dx = house->x - bazaar_x;
        if (dx < 0) dx = -dx;
        dy = house->y - bazaar_y;
        if (dy < 0) dy = -dy;
        if ((dx > dy ? dx : dy) > radius) continue;

        level = house->subtype;
        if (level < 19) ++level;
        amount = house_models[level].beer;
        if (house->size > 1u) amount <<= 2;
        if (amount != 0) buildings[bazaar_id].bazaar_beer_supply = 10;
        amount = house_models[level].pottery;
        if (house->size > 1u) amount <<= 2;
        if (amount != 0) buildings[bazaar_id].bazaar_pottery_supply = 10;
        amount = house_models[level].linen;
        if (house->size > 1u) amount <<= 2;
        if (amount != 0) buildings[bazaar_id].bazaar_linen_supply = 10;
        amount = house_models[level].jewelry;
        if (house->size > 1u) amount <<= 2;
        if (amount != 0) buildings[bazaar_id].bazaar_jewelry_supply = 10;
    }
}

int32_t pharaoh_building_market_max_food_stock(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    const int32_t *resource_ids, int32_t resource_count)
{
    int32_t result = 0;
    const uint8_t *record;
    if (building_id < 1 ||
        (buildings[building_id].building_type != 70 &&
         buildings[building_id].building_type != 190)) return 0;
    record = (const uint8_t *)&buildings[building_id];
    for (int32_t i = 0; i < resource_count; ++i) {
        if (resource_ids[i] != 0) {
            const int32_t slot = market_food_slot(resource_ids[i]);
            const int32_t value = *(const int16_t *)(record + 0x56 + slot * 2);
            if (value > result) result = value;
        }
    }
    return result;
}

int32_t pharaoh_building_market_max_goods_stock(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id)
{
    int32_t result = 0;
    const uint8_t *record;
    if (building_id < 1 ||
        (buildings[building_id].building_type != 70 &&
         buildings[building_id].building_type != 190)) return 0;
    record = (const uint8_t *)&buildings[building_id];
    for (int32_t offset = 0x66; offset <= 0x6c; offset += 2) {
        const int32_t value = *(const int16_t *)(record + offset);
        if (value > result) result = value;
    }
    return result;
}

void pharaoh_house_population_rebuild_list(
    const PharaohBuildingTypeRecord *buildings, int32_t highest_id,
    int16_t *house_ids, int32_t *house_count)
{
    int32_t count = 0;
    *house_count = 0;
    for (int32_t id = 1; id <= highest_id; ++id) {
        if (buildings[id].state == 1 && buildings[id].house_size != 0) {
            house_ids[count++] = (int16_t)id;
        }
    }
    *house_count = count;
}

int32_t pharaoh_house_population_closest_with_room(
    const PharaohBuildingTypeRecord *buildings, int32_t highest_id,
    int32_t x, int32_t y)
{
    int32_t closest_id = 0;
    int32_t closest_distance = 1000;
    for (int32_t id = 1; id <= highest_id; ++id) {
        const PharaohBuildingTypeRecord *building = &buildings[id];
        int32_t dx;
        int32_t dy;
        int32_t distance;
        if (building->state != 1 || building->house_size == 0 ||
            building->distance_from_entry <= 0 ||
            building->house_population_room <= 0 ||
            building->immigrant_walker_id != 0) continue;
        dx = building->x < x ? x - building->x : building->x - x;
        dy = building->y < y ? y - building->y : building->y - y;
        distance = dx < dy ? dy : dx;
        if (distance < closest_distance) {
            closest_distance = distance;
            closest_id = id;
        }
    }
    return closest_id;
}

int32_t pharaoh_house_population_add(
    PharaohBuildingTypeRecord *buildings,
    const PharaohHouseModel *house_models,
    PharaohHousePopulationPlayerMetrics *player, int32_t requested)
{
    int32_t added = 0;
    int32_t id = player->last_used_house_add;
    for (int32_t attempt = 1; attempt < 4000; ++attempt) {
        PharaohBuildingTypeRecord *building;
        int32_t capacity;
        int16_t population;
        if (requested <= added) return added;
        if (++id > 3999) id = 1;
        building = &buildings[id];
        population = building->house_population;
        if (building->state != 1 || building->house_size == 0 ||
            building->distance_from_entry <= 0 || population <= 0) continue;
        player->last_used_house_add = id;
        capacity = house_models[building->subtype].max_people;
        if (building->house_is_merged != 0) capacity *= 4;
        if (capacity > population) {
            ++added;
            building->house_population = population + 1;
            building->house_population_room =
                (int16_t)(capacity - (population + 1));
        }
    }
    return added;
}

int32_t pharaoh_house_population_remove(
    PharaohBuildingTypeRecord *buildings,
    PharaohHousePopulationPlayerMetrics *player, int32_t requested)
{
    int32_t removed = 0;
    int32_t id = player->last_used_house_remove;
    for (int32_t attempt = 1; attempt < 16000; ++attempt) {
        PharaohBuildingTypeRecord *building;
        if (requested <= removed) return removed;
        if (++id > 3999) id = 1;
        building = &buildings[id];
        if (building->state != 1 || building->house_size == 0) continue;
        player->last_used_house_remove = (int16_t)id;
        if (building->house_population > 0) {
            ++removed;
            --building->house_population;
        }
    }
    return removed;
}

int32_t pharaoh_house_population_calculate_types(
    const PharaohBuildingTypeRecord *buildings, int32_t highest_id,
    PharaohHousePopulationPlayerMetrics *player)
{
    int32_t total = 0;
    player->people_in_huts_and_shanties = 0;
    player->people_in_manors_and_up = 0;
    player->people_in_huts = 0;
    player->people_in_residences_and_up = 0;
    for (int32_t id = 1; id <= highest_id; ++id) {
        const PharaohBuildingTypeRecord *building = &buildings[id];
        const uint8_t state = building->state;
        const int32_t population = building->house_population;
        const int16_t level = building->subtype;
        if (state == 0 || state == 5 || state == 2 || state == 6 ||
            building->house_size == 0) continue;
        total += population;
        if (level < 2) player->people_in_huts += population;
        if (level < 4) player->people_in_huts_and_shanties += population;
        if (level > 9) player->people_in_residences_and_up += population;
        if (level > 13) player->people_in_manors_and_up += population;
    }
    return total;
}

void pharaoh_house_service_decay_tax_collector(
    PharaohBuildingTypeRecord *buildings, int32_t highest_id)
{
    for (int32_t id = 1; id <= highest_id; ++id) {
        if (buildings[id].state == 1 &&
            buildings[id].tax_collector_service != 0) {
            --buildings[id].tax_collector_service;
        }
    }
}

int32_t pharaoh_building_is_mansion(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id)
{
    const int32_t type = buildings[building_id].building_type;
    return type >= 0x4d && type <= 0x4f;
}

int32_t pharaoh_building_has_special_service_kind(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id)
{
    switch (buildings[building_id].building_type) {
        case 0x4d:
        case 0x4e:
        case 0x4f:
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x57:
        case 0xb8:
        case 0xbb:
        case 0xbc:
        case 0xbd:
            return 1;
        default:
            return 0;
    }
}

static int32_t house_expansion_neighbour_matches(
    const PharaohHouseExpansionRecord *buildings, int32_t tile_building_id,
    int32_t house_id)
{
    const PharaohHouseExpansionRecord *house = &buildings[house_id];
    const PharaohHouseExpansionRecord *other;
    if (tile_building_id == house_id) return 1;
    other = &buildings[tile_building_id];
    return other->state == 1 && other->house_size != 0 &&
        other->subtype <= house->subtype;
}

static int32_t building_coordinate_difference(int32_t a, int32_t b)
{
    if (a > b) return (int32_t)((uint32_t)a - (uint32_t)b);
    if (a < b) return (int32_t)((uint32_t)b - (uint32_t)a);
    return 0;
}

int32_t pharaoh_shipwright_order_rank(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    const PharaohShipwrightOrderRecord *orders)
{
    const PharaohBuildingTypeRecord *building = &buildings[building_id];
    if (building->building_type != 0x4a || building->state != 1u ||
        building->repair_queue_id == 0u) return -1;
    return orders[building->repair_queue_id].rank;
}

void pharaoh_building_toggle_orders_view(PharaohBuildingTypeRecord *buildings,
    int32_t building_id, uint8_t *redraw_requested)
{
    if (buildings[building_id].orders_view == 1)
        buildings[building_id].orders_view = 16;
    else if (buildings[building_id].orders_view == 16)
        buildings[building_id].orders_view = 1;
    *redraw_requested = 1;
}

int32_t pharaoh_shipwright_boat_destination(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    uint32_t boat_type)
{
    int32_t building_type;
    int32_t id;

    if (boat_type == 25u) building_type = 76;
    else building_type = boat_type == 77u ? 181 : 182;

    for (id = 1; id < PHARAOH_BUILDING_RECORD_COUNT; ++id) {
        PharaohBuildingTypeRecord *building = &buildings[id];
        int32_t walker_id;

        if (building->state == 0u || building->building_type != building_type)
            continue;
        walker_id = building->shipwright_boat_id;
        if (walker_id < 1) return id;
        if (walkers[walker_id].state == 0u ||
            (walkers[walker_id].type != boat_type &&
             walkers[walker_id].type != 0u) ||
            walkers[walker_id].building_id != id) {
            building->shipwright_boat_id = 0;
            return id;
        }
    }
    return 0;
}

void pharaoh_entertainer_update_show_days(
    const PharaohWalkerQueryRecord *walkers,
    PharaohBuildingTypeRecord *buildings, int32_t walker_id)
{
    const PharaohWalkerQueryRecord *w = &walkers[walker_id];
    uint8_t *b = (uint8_t *)&buildings[w->entertainment_building_id];
    if (w->type == 0x0f) b[0xa7] = 0x20;
    else if (w->type == 0x10) b[0xa8] = 0x20;
    else if (w->type == 0x11) b[0xa9] = 0x20;
}

uint8_t pharaoh_building_type_is_not_monument_group_004696b0(
    const PharaohBuildingTypeRecord *buildings, int32_t id)
{
    switch (buildings[id].building_type) {
    case 0xb7: case 0xcf: case 0xd2: case 0xd5: case 0xd7: case 0xda:
    case 0xdb: case 0xdc: case 0xde: case 0xe1: case 0xe3: case 0xe4:
    case 0xe5: case 0xe6: case 0xea: case 0xeb: case 0xec: return 0;
    default: return 1;
    }
}

int32_t pharaoh_building_maintenance_get_closest_burning_ruin(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const int16_t *burning_building_ids, int32_t burning_building_count,
    int32_t x, int32_t y, PharaohFireSearchState *search)
{
    int32_t unassigned = 0;
    int32_t assigned = 0;
    int32_t assigned_distance = 10000;
    int32_t index;

    search->best_unassigned_distance = 10000;
    search->search_remaining = 0;
    if (burning_building_count <= 0) return 0;
    search->search_remaining = burning_building_count;
    for (index = 0; index < burning_building_count; ++index) {
        const int32_t building_id = burning_building_ids[index];
        const PharaohBuildingTypeRecord *building = &buildings[building_id];
        int32_t dx, dy, distance;
        if (building->state != 1 || building->building_type != 99 ||
            building->ruin_has_plague != 0 ||
            building->distance_from_entry == 0) continue;
        dx = building_coordinate_difference(x, building->x);
        dy = building_coordinate_difference(y, building->y);
        search->distance_x = dx;
        search->distance_y = dy;
        distance = dx >= dy ? dx : dy;
        if (building->walker_id4 != 0) {
            if (distance < assigned_distance) {
                assigned_distance = distance;
                assigned = building_id;
            }
        } else if (distance < search->best_unassigned_distance) {
            search->best_unassigned_distance = distance;
            unassigned = building_id;
        }
    }
    if (unassigned == 0 && assigned_distance < 3) return assigned;
    return unassigned;
}

uint32_t pharaoh_building_house_can_expand(
    PharaohHouseExpansionRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const uint32_t *terrain, const int16_t *building_ids,
    const int32_t direction_offsets[4], const int32_t *tile_offsets,
    const int32_t expansion_deltas[8], int32_t building_id,
    int32_t tile_count, int32_t *expansion_x, int32_t *expansion_y)
{
    PharaohHouseExpansionRecord *house = &buildings[building_id];
    int32_t pass;
    for (pass = 0; pass < 3; ++pass) {
        int32_t direction;
        for (direction = 0; direction < 4; ++direction) {
            const int32_t base = house->grid_offset + direction_offsets[direction];
            int32_t count = 0;
            int32_t index;
            for (index = 0; index < tile_count; ++index) {
                const int32_t tile = base + tile_offsets[index];
                const uint32_t flags = terrain[tile];
                if (pass == 0) {
                    if ((flags & 8u) != 0 &&
                        house_expansion_neighbour_matches(buildings,
                            building_ids[tile], building_id)) ++count;
                } else {
                    const uint32_t blocked = flags & 0xeefed77fu;
                    if (blocked == 0) ++count;
                    if ((blocked & 8u) != 0) {
                        if (house_expansion_neighbour_matches(buildings,
                                building_ids[tile], building_id)) ++count;
                    } else if (pass == 2 && (blocked & 0x20u) != 0) {
                        ++count;
                    }
                }
            }
            if (count == tile_count) {
                *expansion_x = house->x + expansion_deltas[direction * 2];
                *expansion_y = house->y + expansion_deltas[direction * 2 + 1];
                return 1;
            }
        }
    }
    house->no_space_to_expand = 1;
    return 0;
}

void pharaoh_building_house_repair_map_position(
    PharaohHouseExpansionRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const int16_t *building_ids, int32_t building_id,
    int32_t grid_start, int32_t grid_width, int32_t grid_height,
    int32_t grid_border_size, uint32_t *repair_attempts,
    uint32_t *repair_failures)
{
    PharaohHouseExpansionRecord *house = &buildings[building_id];
    int32_t tile = house->grid_offset;
    int32_t row;

    house->no_space_to_expand = 0;
    if (tile == grid_start + house->y * 228 + house->x &&
        building_ids[tile] == building_id) return;

    ++*repair_attempts;
    tile = grid_start;
    for (row = 0; row < grid_height; ++row) {
        int32_t column;
        for (column = 0; column < grid_width; ++column, ++tile) {
            if (building_ids[tile] == building_id) {
                const int32_t relative = tile - grid_start;
                house->grid_offset = tile;
                house->x = (int16_t)(relative % 228);
                house->y = (int16_t)(relative / 228);
                return;
            }
        }
        tile += grid_border_size;
    }
    house->state = 4;
    ++*repair_failures;
}

int32_t pharaoh_building_house_collect_expansion_stock(
    PharaohHouseExpansionRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const uint32_t *terrain, const int16_t *building_ids,
    const int32_t *tile_offsets, int32_t map_origin,
    int32_t expansion_x, int32_t expansion_y,
    int32_t building_id, int32_t tile_count,
    uint32_t stock_totals[12])
{
    int32_t population = 0;
    const int32_t base = map_origin + expansion_y * 228 + expansion_x;
    int32_t slot;
    int32_t index;

    for (slot = 0; slot < 12; ++slot) stock_totals[slot] = 0;
    for (index = 0; index < tile_count; ++index) {
        const int32_t tile = base + tile_offsets[index];
        int32_t donor_id;
        PharaohHouseExpansionRecord *donor;
        if ((terrain[tile] & 8u) == 0) continue;
        donor_id = building_ids[tile];
        if (donor_id == building_id) continue;
        donor = &buildings[donor_id];
        if (donor->house_size == 0) continue;
        population += donor->house_population;
        for (slot = 0; slot < 12; ++slot)
            stock_totals[slot] += (uint32_t)(int32_t)donor->inventory[slot];
        donor->house_population = 0;
        donor->state = 5;
    }
    return population;
}

void pharaoh_city_ratings_prosperity_cap(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t highest_id, const PharaohHouseModel *house_models,
    int32_t *prosperity_cap)
{
    uint32_t weighted_total = 0;
    int32_t population = 0;
    int32_t id;

    for (id = 1; id <= highest_id; ++id) {
        const PharaohBuildingTypeRecord *building = &buildings[id];
        if (building->state != 0 && building->house_size != 0) {
            const int32_t house_population = building->house_population;
            weighted_total += (uint32_t)house_models[building->subtype].prosperity *
                (uint32_t)house_population;
            population += house_population;
        }
    }
    *prosperity_cap = population > 0 ?
        (int32_t)weighted_total / population : 0;
}

int32_t pharaoh_building_house_tax_income(
    const PharaohBuildingTypeRecord *buildings, int32_t id)
{
    const PharaohBuildingTypeRecord *b = &buildings[id];
    if (BUILDING_BYTE(b, 5) == 0) return 0;
    return (BUILDING_DWORD(b, 0x8c) + 5000) / 10000;
}

int32_t pharaoh_dock_registry_index(int32_t dock_count,
    const int16_t dock_ids[PHARAOH_DOCK_REGISTRY_CAPACITY],
    int32_t building_id)
{
    int32_t index;
    if (dock_count <= 0) return -1;
    for (index = 0; index < PHARAOH_DOCK_REGISTRY_CAPACITY; ++index)
        if (dock_ids[index] == building_id) return index;
    return -1;
}

bool pharaoh_ship_is_docked(const PharaohWalkerQueryRecord *walkers,
    const PharaohBuildingTypeRecord *buildings, int32_t walker_id)
{
    const int32_t building_id = walkers[walker_id].building_id;
    return building_id != 0 &&
        buildings[building_id].fishing_boat_id == walker_id;
}

int32_t pharaoh_formation_legion_at_building(
    const int16_t *building_id_grid,
    const PharaohBuildingTypeRecord *buildings, int32_t grid_offset)
{
    const int32_t id = building_id_grid[grid_offset];
    const uint8_t *b;
    if (id <= 0 || buildings[id].state != 1) return 0;
    if (buildings[id].building_type != 0x36 && buildings[id].building_type != 0x39) return 0;
    b = (const uint8_t *)&buildings[id];
    return *(const int16_t *)(b + 0x50);
}

int32_t pharaoh_building_type_group_index(int32_t type)
{
    switch (type) {
    case 0x3c: case 0x41: return 0;
    case 0x3d: case 0x42: return 1;
    case 0x3e: case 0x43: return 2;
    case 0x3f: case 0x44: return 3;
    case 0x40: case 0x45: return 4;
    default: return -1;
    }
}

void pharaoh_mausoleum_entry_tile(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    int32_t *x, int32_t *y)
{
    *x = buildings[building_id].origin_x + 22;
    *y = buildings[building_id].origin_y + 4;
}

int32_t pharaoh_building_obelisk_part_number(
    const PharaohBuildingTypeRecord *buildings, int32_t id)
{
    const PharaohBuildingTypeRecord *b = &buildings[id];
    const int32_t parts = b->building_type == 0xcf ? 10 : 18;
    int32_t result = (BUILDING_BYTE(b, 0x55) - 0x21) * parts / 0x42 + 1;
    return result < parts ? result : parts;
}

void pharaoh_building_adjust_active_rating_004e8e80(
    PharaohBuildingTypeRecord *buildings, int8_t delta)
{
    int32_t id;
    for (id = 1; id < PHARAOH_BUILDING_RECORD_COUNT; ++id) {
        PharaohBuildingTypeRecord *b = &buildings[id];
        if (b->state == 1 && BUILDING_BYTE(b, 5) != 0) {
            int32_t value = (int8_t)(uint8_t)(
                BUILDING_BYTE(b, 0xc2) + (uint8_t)delta);
            if (value < 0) value = 0;
            if (value > 100) value = 100;
            *((uint8_t *)b + 0xc2) = (uint8_t)value;
        }
    }
}

TYPE_IN_CASES(pharaoh_building_type_is_004eada0_group,
    case 0x19: case 0x4d: case 0x4e: case 0x5c: case 0x5d: case 100: case 0x65:)

int32_t pharaoh_laborer_pyramid_set_layout_target(
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t walker_id, int32_t building_id, int32_t pyramid_style,
    const int32_t *mastaba_first_part_by_size,
    const int32_t *pyramid_first_part_by_size)
{
    const int32_t root_id = building_id - buildings[building_id].subtype;
    const uint8_t size = buildings[root_id].pyramid_size_variant;
    const int32_t part = pyramid_style == 5 ?
        mastaba_first_part_by_size[size] : pyramid_first_part_by_size[size];
    const int32_t target_id = root_id + part;
    PharaohWalkerQueryRecord *walker = &walkers[walker_id];
    const PharaohBuildingTypeRecord *target = &buildings[target_id];
    walker->destination_x = target->road_access_x;
    walker->destination_y = target->road_access_y;
    walker->destination_grid_offset = target->grid_offset;
    walker->roam_length = 0;
    return target_id;
}

int32_t pharaoh_building_subtype(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id)
{
    return buildings[building_id].subtype;
}

uint32_t pharaoh_building_advisor_option_category(uint32_t building_type)
{
    switch (building_type) {
    case 30:
    case 62: return 13;
    case 31:
    case 32:
    case 48:
    case 53:
    case 54:
    case 55:
    case 56: return 8;
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38: return 10;
    case 39:
    case 40:
    case 41:
    case 49:
    case 57:
    case 59: return 6;
    case 44:
    case 45:
    case 46: return 9;
    case 50:
    case 51:
    case 52: return 11;
    default: return 0;
    }
}

uint8_t pharaoh_sun_temple_access_tile(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    const PharaohSunTemplePartDefinition *part_definitions,
    int32_t *x, int32_t *y)
{
    const PharaohBuildingTypeRecord *building = &buildings[building_id];
    const int32_t subtype = building->subtype;
    switch (part_definitions[subtype].size) {
        case 0: *x = building->x + 2; *y = building->y + 5; break;
        case 1: *x = building->x + 1; *y = building->y + 2; break;
        case 2: *x = building->x + 1; *y = building->y + 1; break;
        case 3: *x = building->x - 1; *y = building->y + 1; break;
        case 4: *x = building->x - 1; *y = building->y - 1; break;
        case 5: *x = building->x + 1; *y = building->y - 1; break;
        case 6:
            *x = building->x + (((uint16_t)subtype & 1u) == 0u ? 1 : -1);
            *y = building->y;
            break;
        case 12: *x = building->x + 1; *y = building->y + 1; break;
        default: *x = building->x; *y = building->y; break;
    }
    return 1;
}
