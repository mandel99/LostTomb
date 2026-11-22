#include "pharaoh/building/type_queries.h"

#include "pharaoh/walker/record_queries.h"


/* Native0x00485F70; completed C owner reused with explicit state. */
void pharaoh_city_rebuild_building_tiles_and_walker_chains(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    const int32_t *building_properties, const int32_t *current_player,
    int32_t *construction_size,
    PharaohBuildingConstructionAddToMap add_to_map, void *user_data)
{
    int32_t building_id;
    int32_t walker_id;

    for (building_id = 1; building_id < PHARAOH_BUILDING_RECORD_COUNT;
         ++building_id) {
        const PharaohBuildingTypeRecord *const building =
            &buildings[building_id];
        if (building->state != 0u) {
            const int32_t type = building->building_type;
            *construction_size = building_properties[type * 5];
            add_to_map(user_data, *current_player, type, building_id,
                building->x, building->y, *construction_size, 0, 0);
        }
    }
    for (walker_id = 1; walker_id < PHARAOH_WALKER_RECORD_COUNT;
         ++walker_id) {
        if (walkers[walker_id].state != 0u) {
            int32_t previous = walker_id;
            int32_t next = walkers[walker_id].next_walker_id_on_same_tile;
            while (next != 0) {
                if (next == walker_id) {
                    walkers[previous].next_walker_id_on_same_tile = 0;
                    break;
                }
                previous = next;
                next = walkers[next].next_walker_id_on_same_tile;
            }
        }
    }
}

/* Native0x0045AA30; completed C owner reused with explicit state. */
void pharaoh_building_update_highest_ids(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const PharaohLegacyBuildingRecord legacy_buildings[200],
    int32_t *highest_id, int32_t *highest_id_ever,
    int32_t *legacy_highest_id)
{
    int32_t id;
    *highest_id = 0;
    for (id = 1; id < PHARAOH_BUILDING_RECORD_COUNT; ++id) {
        if (buildings[id].state != 0u) *highest_id = id;
    }
    if (*highest_id_ever < *highest_id) *highest_id_ever = *highest_id;
    *legacy_highest_id = 0;
    for (id = 1; id < 200; ++id) {
        if (legacy_buildings[id].state != 0u) *legacy_highest_id = id;
    }
}

static int32_t building_destination_kind_counted(int32_t kind)
{
    if (kind >= 110 && kind <= 114) return 1;
    switch (kind) {
        case 32: case 47: case 51: case 53: case 74: case 113:
        case 177: case 178: case 187: case 188: case 189: case 194:
        case 203: case 204: case 205: case 226: case 231: case 232:
        case 233:
            return 1;
        default:
            return 0;
    }
}

/* Native0x004BC7A0; completed C owner reused with explicit state. */
void pharaoh_building_destination_counters_rebuild(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT])
{
    int32_t building_id;
    int32_t walker_id;
    for (building_id = 1; building_id < PHARAOH_BUILDING_RECORD_COUNT;
         ++building_id) {
        PharaohBuildingTypeRecord *building = &buildings[building_id];
        const int32_t kind = building->building_type;
        if (building->state == 1u &&
            (building_destination_kind_counted(kind) ||
             kind == 0x47 || kind == 0x5f)) {
            building->destination_walker_count = 0;
            building->destination_payload_total = 0;
        }
    }
    for (walker_id = 1; walker_id < PHARAOH_WALKER_RECORD_COUNT;
         ++walker_id) {
        const PharaohWalkerQueryRecord *walker = &walkers[walker_id];
        PharaohBuildingTypeRecord *destination;
        int32_t kind;
        if (walker->state == 0u || walker->destination_payload_074 == 0u)
            continue;
        destination = &buildings[walker->entertainment_building_id];
        kind = destination->building_type;
        if (building_destination_kind_counted(kind)) {
            if (kind == 0x5f) {
                if (walker->counting_action_00b == 0x1c)
                    ++destination->destination_payload_total;
                else if (walker->counting_action_00b == 0x0a)
                    ++destination->destination_walker_count;
            } else if (kind == 0xcc) {
                if (walker->counting_action_00b == 9u)
                    ++destination->destination_payload_total;
                else if (walker->counting_action_00b == 11u)
                    ++destination->destination_walker_count;
            } else {
                ++destination->destination_walker_count;
            }
        } else if (kind == 0x47) {
            ++destination->destination_walker_count;
            destination->destination_payload_total = (int16_t)(
                (uint16_t)destination->destination_payload_total +
                walker->destination_payload_074);
        }
    }
}

/* Native0x004D5BC0; completed C owner reused with explicit state. */
void pharaoh_city_find_type_d1_anchor(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t *anchor_x, int32_t *anchor_y)
{
    int32_t id;
    for (id = 0; id < PHARAOH_BUILDING_RECORD_COUNT; ++id) {
        if (buildings[id].state == 1u &&
            buildings[id].building_type == 0xd1) {
            *anchor_x = buildings[id].x;
            *anchor_y = buildings[id].y;
            return;
        }
    }
    *anchor_x = -1;
    *anchor_y = -1;
}


/* Native0x004BCA90; completed C owner reused with explicit state. */
void pharaoh_walker_count_active_transport_types(
    const PharaohWalkerQueryRecord *walkers, int32_t *count)
{
    int32_t id;
    *count = 0;
    for (id = 1; id < PHARAOH_WALKER_RECORD_COUNT; ++id)
        if (walkers[id].state != 0 &&
            (walkers[id].type == 0x5d || walkers[id].type == 0x64)) ++*count;
}

/* Native0x004B32C0; completed C owner reused with explicit state. */
void pharaoh_walker_route_clean(
    int16_t route_walker_ids[1000], const PharaohWalkerQueryRecord *walkers)
{
    int32_t route_id;
    for (route_id = 1; route_id < 1000; ++route_id) {
        const int32_t walker_id = route_walker_ids[route_id];
        if (walker_id > 0 && walker_id < PHARAOH_WALKER_RECORD_COUNT &&
            (walkers[walker_id].state != 1 ||
             walkers[walker_id].slot_link_id != route_id))
            route_walker_ids[route_id] = 0;
    }
}

/* Native0x004A3560; completed C owner reused with explicit state. */
void pharaoh_ferry_check_passengers(
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    const PharaohFerryPathRecord paths[50])
{
    for (int32_t walker_id = 1;
         walker_id < PHARAOH_WALKER_RECORD_COUNT; ++walker_id) {
        PharaohWalkerQueryRecord *walker = &walkers[walker_id];
        if (walker->state == 0 || walker->action_state != 6) continue;

        int32_t path_id;
        for (path_id = 0; path_id < 50; ++path_id) {
            const PharaohFerryPathRecord *path = &paths[path_id];
            if (path->active == 0) continue;

            int32_t child_slot;
            for (child_slot = 0; child_slot < 4; ++child_slot) {
                if (path->child_walker_ids[child_slot] == walker_id) break;
            }
            if (child_slot == 4) continue;

            int32_t owner_id;
            for (owner_id = 1; owner_id < PHARAOH_WALKER_RECORD_COUNT;
                 ++owner_id) {
                const PharaohWalkerQueryRecord *owner = &walkers[owner_id];
                if (owner->state != 0 && owner->type == 0x4c &&
                    owner->formation_id == path_id) {
                    break;
                }
            }
            if (owner_id < PHARAOH_WALKER_RECORD_COUNT) break;
            path_id = 50;
            break;
        }
        if (path_id == 50) walker->state = 2;
    }
}
