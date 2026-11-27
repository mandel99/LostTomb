#include "pharaoh/ui/city_undo_live.h"
#include "pharaoh/commerce/granary_repair.h"
#include "pharaoh/monuments/tile_removal.h"

#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohCityUndoFormation) == 0x90 &&
    offsetof(PharaohCityUndoFormation, legion_id) == 2 &&
    offsetof(PharaohCityUndoFormation, building_id) == 6 &&
    offsetof(PharaohCityUndoFormation, is_legion) == 0x42,
    "native undo formation fields");

static int32_t signed_bits(uint32_t bits)
{
    return bits <= INT32_MAX ? (int32_t)bits :
        (int32_t)((int64_t)bits - INT64_C(0x100000000));
}

static int32_t add(int32_t a, int32_t b)
{ return signed_bits((uint32_t)a + (uint32_t)b); }
static int32_t sub(int32_t a, int32_t b)
{ return signed_bits((uint32_t)a - (uint32_t)b); }

static int32_t storage_resource_limit(
    const PharaohBuildingStorageRecord *storages, uint8_t slot,
    int32_t resource)
{
    const PharaohBuildingStorageRecord *storage = &storages[slot];
    return storage->resource_state[resource] != 0 ?
        storage->get_amount[resource] : storage->accept_amount[resource];
}

uint8_t pharaoh_city_undo_storage_limit_exceeded(
    const PharaohBuildingTypeRecord *buildings,
    const PharaohBuildingStorageRecord *storages,
    int32_t building, int32_t amount, int32_t resource)
{
    const int32_t limit = storage_resource_limit(storages,
        buildings[building].storage_id, resource);
    const int32_t quantity = pharaoh_storageyard_resource_amount(
        buildings, building, resource);
    return (uint8_t)(limit < signed_bits((uint32_t)quantity + (uint32_t)amount));
}

static uint16_t storage_word_04a(const PharaohBuildingTypeRecord *building)
{
    uint16_t value;
    memcpy(&value, (const uint8_t *)building + 0x4a, sizeof(value));
    return value;
}

int32_t pharaoh_city_undo_storage_deliver(const PharaohCityUndoStorageContext *c,
    uint32_t player, int32_t building, int32_t resource, int32_t amount)
{
    const int32_t root = pharaoh_building_follow_link(c->buildings, building);
    const uint8_t slot = c->buildings[root].storage_id;
    uint32_t overflow = 0, delivered = 0;
    if (c->buildings[root].num_workers <= 0 ||
        storage_word_04a(&c->buildings[root]) != 0) return 0;
    if (pharaoh_city_undo_storage_limit_exceeded(c->buildings, c->storages,
        root, 0, resource)) return 0;
    const int32_t limit = storage_resource_limit(c->storages, slot, resource);
    if (pharaoh_city_undo_storage_limit_exceeded(c->buildings, c->storages,
        root, amount, resource)) {
        overflow = (uint32_t)pharaoh_storageyard_resource_amount(
            c->buildings, root, resource) + (uint32_t)amount - (uint32_t)limit;
    }
    const int32_t remaining = signed_bits((uint32_t)amount - overflow);
    if (remaining <= 0) return remaining;
    do {
        int32_t node = pharaoh_building_follow_link(c->buildings, building);
        int32_t chosen = 0, best = -1;
        for (int32_t index = 0; index < 8; ++index) {
            node = c->buildings[node].next_storage_building_id;
            if (node <= 0) return 0;
            const PharaohBuildingTypeRecord *room = &c->buildings[node];
            if (room->subtype != 0 && room->subtype != resource) continue;
            const int32_t stock = room->storage_link_amount;
            if (stock < 400 && stock > best) { best = stock; chosen = node; }
        }
        if (chosen == 0) return 0;
        building = chosen;
        PharaohBuildingTypeRecord *room = &c->buildings[chosen];
        const uint32_t counter = player * UINT32_C(0x1276) + (uint32_t)resource;
        c->stock->withdrawn[counter] -= 1u;
        c->stock->stored[counter] += 1u;
        room->subtype = (int16_t)resource;
        room->storage_link_amount = (int16_t)(uint16_t)
            ((uint16_t)room->storage_link_amount + 1u);
        const uint32_t image = resource < 31 ?
            *c->stock->pile_image_first + (uint32_t)resource * 4u - 4u :
            *c->stock->extended_pile_image_first + (uint32_t)resource * 4u - 124u;
        c->stock->tile_images[room->grid_offset] = image +
            (uint32_t)pharaoh_storage_image_level(room->storage_link_amount);
        ++delivered;
    } while (signed_bits(delivered) < remaining);
    return remaining;
}

/* A selected-record view for the existing normalized45E220 algorithm.
   It observes only positive identity, type, slot policy and stock; mapping
   the single selected row to index1 has no identity side effect. This is
   not a world copy. Only the two actually changed Native WORDs are returned. */
static int32_t granary_add_actual(const PharaohCityUndoStorageContext *c,
    int32_t building, int32_t resource, int32_t amount)
{
    PharaohGranaryRepairBuilding selected[2];
    PharaohGranaryRepairStorage policy[1];
    PharaohBuildingTypeRecord *actual = &c->buildings[building];
    const PharaohBuildingStorageRecord *storage = &c->storages[actual->storage_id];
    selected[1].state = actual->state;
    selected[1].type = actual->building_type;
    selected[1].storage_id = 0;
    memcpy(selected[1].resource_stored, actual->granary_resource_stored,
        sizeof(selected[1].resource_stored));
    memcpy(policy[0].resource_state, storage->resource_state,
        sizeof(policy[0].resource_state));
    memcpy(policy[0].accept_amount, storage->accept_amount,
        sizeof(policy[0].accept_amount));
    memcpy(policy[0].get_amount, storage->get_amount,
        sizeof(policy[0].get_amount));
    const int32_t result = pharaoh_granary_add_resource(selected, policy,
        1, resource, 0, amount, NULL);
    actual->granary_resource_stored[resource] =
        selected[1].resource_stored[resource];
    actual->granary_resource_stored[0] = selected[1].resource_stored[0];
    return result;
}

int32_t pharaoh_city_undo_store_anywhere(const PharaohCityUndoStorageContext *c,
    int32_t resource, int32_t requested, uint8_t count_only)
{
    int32_t stored = 0, remaining = requested;
    for (int32_t building = 1; building <= *c->highest_building_id; ++building) {
        PharaohBuildingTypeRecord *b = &c->buildings[building];
        if (b->state != 1 || b->has_road_access == 0 ||
            b->distance_from_entry <= 0 || b->num_workers <= 0) continue;
        if (b->building_type != 0x48 &&
            !((b->building_type == 0x47 || b->building_type == 0xbf) &&
                resource > 0 && resource < 9)) continue;
        const PharaohBuildingStorageRecord *storage = &c->storages[b->storage_id];
        const uint8_t policy = storage->resource_state[resource];
        if (policy != 0 && policy != 2) continue;
        const int32_t limit = policy == 0 ? storage->accept_amount[resource] :
            storage->get_amount[resource];
        int32_t available;
        if (b->building_type == 0x48) {
            available = 0;
            int32_t room = b->next_storage_building_id;
            while (room != 0) {
                const PharaohBuildingTypeRecord *part = &c->buildings[room];
                if (part->subtype == 0) available = add(available, 400);
                else if (part->subtype == resource) {
                    available = add(available, sub(400, part->storage_link_amount));
                    remaining = requested;
                }
                room = part->next_storage_building_id;
            }
            if (limit <= available) available = limit;
            if (remaining <= available) available = remaining;
            if (count_only == 0)
                (void)pharaoh_city_undo_storage_deliver(c, *c->current_player,
                    building, resource, available);
        } else if ((b->building_type == 0x47 || b->building_type == 0xbf) &&
            resource > 0 && resource < 9) {
            available = sub(limit, b->granary_resource_stored[resource]);
            if (b->granary_resource_stored[0] <= available)
                available = b->granary_resource_stored[0];
            if (available <= 0) continue;
            if (remaining <= available) available = remaining;
            if (count_only == 0)
                (void)granary_add_actual(c, building, resource, available);
        } else continue;
        remaining = sub(remaining, available);
        stored = add(stored, available);
        requested = remaining;
        if (remaining < 1) return stored;
    }
    return stored;
}

static uint8_t storage_kind(int16_t kind)
{
    return (uint8_t)(kind == 0x47 || kind == 0x48 || kind == 0x46 ||
        kind == 0xbf || kind == 0xc1 || kind == 0xbe);
}

void pharaoh_city_undo_storage_reset_links(
    const PharaohCityUndoMaintenanceContext *c)
{
    PharaohConstructionSupportContext *support = c->support;
    PharaohBuildingTypeRecord *buildings = support->surface->buildings;
    for (int32_t slot = 1; slot < 200; ++slot)
        support->storages[slot].building_id = 0;
    const int32_t initial_bound = *c->highest_building_id;
    for (int32_t id = 1; id <= initial_bound; ++id) {
        const PharaohBuildingTypeRecord *b = &buildings[id];
        if (b->state != 0 && storage_kind(b->building_type) && b->storage_id != 0)
            support->storages[b->storage_id].in_use = 1;
    }
    for (int32_t id = 1; id <= *c->highest_building_id; ++id) {
        PharaohBuildingTypeRecord *b = &buildings[id];
        if (b->state == 0 || !storage_kind(b->building_type)) continue;
        const uint8_t slot = b->storage_id;
        if (slot == 0)
            b->storage_id = (uint8_t)pharaoh_construction_storage_allocate(support, id);
        else if (support->storages[slot].building_id == 0)
            support->storages[slot].building_id = (int16_t)id;
        else b->storage_id = (uint8_t)pharaoh_construction_storage_allocate(support, id);
    }
    *support->highest_storage = 0;
    for (int32_t slot = 1; slot < 200; ++slot)
        if (support->storages[slot].in_use != 0) *support->highest_storage = slot;
}

void pharaoh_city_undo_ferry_reset_links(const PharaohCityUndoMaintenanceContext *c)
{
    PharaohConstructionSupportContext *support = c->support;
    PharaohBuildingTypeRecord *buildings = support->surface->buildings;
    for (int32_t slot = 1; slot < 50; ++slot)
        support->ferry_queues[slot].building_id = 0;
    const int32_t initial_bound = *c->highest_building_id;
    for (int32_t id = 1; id <= initial_bound; ++id) {
        const PharaohBuildingTypeRecord *b = &buildings[id];
        if (b->state != 0 && b->building_type == 0x88 && b->runtime_value_ca != 0)
            support->ferry_queues[b->runtime_value_ca].queue.state = 1;
    }
    if (initial_bound > 0) {
        for (int32_t id = 1; id <= *c->highest_building_id; ++id) {
            PharaohBuildingTypeRecord *b = &buildings[id];
            if (b->state == 0 || b->building_type != 0x88) continue;
            const uint8_t slot = b->runtime_value_ca;
            if (slot == 0) b->runtime_value_ca = (uint8_t)
                pharaoh_construction_ferry_queue_allocate(support, (uint32_t)id);
            else if (support->ferry_queues[slot].building_id == 0)
                support->ferry_queues[slot].building_id = id;
            else b->runtime_value_ca = (uint8_t)
                pharaoh_construction_ferry_queue_allocate(support, (uint32_t)id);
        }
    }
}

void pharaoh_city_undo_repair_reset_links(const PharaohCityUndoMaintenanceContext *c)
{
    PharaohBuildingAllocationContext *allocation = c->allocation;
    PharaohBuildingTypeRecord *buildings = allocation->surface->buildings;
    for (int32_t slot = 0; slot < 10; ++slot)
        allocation->repair_queue[slot].building_id = 0;
    const int32_t initial_bound = *c->highest_building_id;
    for (int32_t id = 1; id <= initial_bound; ++id) {
        const PharaohBuildingTypeRecord *b = &buildings[id];
        if (b->state != 0 && b->building_type == 0x4a && b->repair_queue_id != 0)
            allocation->repair_queue[b->repair_queue_id - 1].active = 1;
    }
    for (int32_t id = 1; id <= *c->highest_building_id; ++id) {
        const PharaohBuildingTypeRecord *b = &buildings[id];
        if (b->state == 0 || b->building_type != 0x4a) continue;
        const uint8_t slot = b->repair_queue_id;
        if (slot == 0)
            pharaoh_building_repair_queue_add(buildings, id, allocation->repair_queue,
                allocation->output_debug, allocation->debug_user);
        else if (allocation->repair_queue[slot - 1].building_id == 0)
            allocation->repair_queue[slot - 1].building_id = id;
        else pharaoh_building_repair_queue_add(buildings, id, allocation->repair_queue,
            allocation->output_debug, allocation->debug_user);
    }
}

void pharaoh_city_undo_restore_houses(PharaohBuildingAllocationContext *allocation,
    PharaohMapBuildingTilesContext *tiles)
{
    for (int32_t id = 1; id < PHARAOH_BUILDING_RECORD_COUNT; ++id) {
        const PharaohBuildingTypeRecord *b = &allocation->surface->buildings[id];
        if (b->state != 0 && b->building_type >= 10 && b->building_type <= 29 &&
            b->house_population == 0)
            pharaoh_house_change_to_vacant_lots(allocation, tiles, id);
    }
}

void pharaoh_city_undo_place_farm(const PharaohFarmTileDrawContext *c,
    int32_t building, uint8_t extra)
{
    const int32_t y = c->surface->buildings[building].y;
    const int32_t x = c->surface->buildings[building].x;
    pharaoh_map_draw_farm(c, building, x, y, extra);
}

void pharaoh_city_undo_mausoleum_images(PharaohMausoleumImageContext *c,
    int32_t building)
{
    const PharaohBuildingTypeRecord *b = &c->surface->buildings[building];
    if (b->building_type != 222) return;
    const int32_t root = sub(building, b->subtype);
    for (int32_t part = 0; part < 116; ++part)
        pharaoh_mausoleum_update_map(c, add(root, part));
}

void pharaoh_city_undo_venue_remove(const PharaohCityUndoLiveContext *c,
    int32_t building, int32_t kind)
{
    PharaohMapSurfaceContext *surface = c->load->surface;
    PharaohMapBuildingTilesContext *tiles = c->load->building_tiles;
    if (surface->buildings[building].building_type == 209)
        c->load->cities[*c->load->current_player].festival_square_placed = 0;
    uint32_t size;
    switch (kind) {
        case 30: size = 3; break;
        case 31: size = 2; break;
        case 33: size = 4; break;
        case 209: size = 5; break;
        default: size = 0; break;
    }
    uint32_t cell = surface->buildings[building].venue_grid_offset;
    const int32_t delta = signed_bits(cell - surface->map->origin);
    const int32_t start_x = delta % 228, start_y = delta / 228;
    for (uint32_t row = 0; row < size; ++row) {
        int32_t x = start_x;
        const int32_t y = add(start_y, (int32_t)row);
        for (uint32_t column = 0; column < size; ++column) {
            uint8_t marker = surface->property_flags[cell];
            tiles->native_adc36c[cell] = 0;
            surface->canal_water[cell] = 0;
            surface->property_flags[cell] = (uint8_t)(marker & 0xe0u);
            marker = surface->edge_flags[cell];
            surface->building_ids[cell] = 0;
            tiles->native_a9cc1c[cell] = 0;
            surface->edge_flags[cell] = (uint8_t)((marker & 0xc0u) | 0x40u);
            const uint32_t flags = surface->map->terrain[cell] & UINT32_C(0x03052ec4);
            surface->image_ids[cell] = 0;
            surface->map->terrain[cell] = flags;
            tiles->native_a9010c[cell] = (uint8_t)kind;
            if ((flags & 64u) != 0) pharaoh_map_set_road_tile(c->roads, x, y, 1);
            else if (*c->paved_refresh_pending != 0)
                pharaoh_map_set_rubble_image(surface, x, y);
            x = add(x, 1);
            ++cell;
        }
        cell += 228u - size;
    }
    const int32_t end_x = add(start_x, (int32_t)size);
    const int32_t end_y = add(start_y, (int32_t)size);
    *c->paved_refresh_pending = 0;
    pharaoh_map_refresh_empty_region(surface, start_x, start_y, end_x, end_y, 0, 0);
    pharaoh_map_refresh_empty_region(surface, sub(start_x, 5), sub(start_y, 5),
        add(end_x, 5), add(end_y, 5), 1, 0);
    pharaoh_map_refresh_meadow(surface, start_x, start_y, end_x, end_y);
    pharaoh_map_refresh_floodplain(surface, start_x, start_y, end_x, end_y);
    pharaoh_map_refresh_water(surface, start_x, start_y, end_x, end_y, 0);
    pharaoh_map_refresh_marshland(surface, start_x, start_y, end_x, end_y);
    pharaoh_map_refresh_rubble(surface, start_x, start_y, end_x, end_y);
    pharaoh_map_refresh_paved_region(surface, start_x, start_y, end_x, end_y, 1);
    pharaoh_map_routing_update_citizen_rect(c->commit->routing,
        start_x, start_y, end_x, end_y, 1);
}

void pharaoh_city_undo_gatehouse_remove(const PharaohCityUndoLiveContext *c,
    int32_t building)
{
    PharaohBuildingTypeRecord *b = &c->load->surface->buildings[building];
    const int32_t x = b->x, y = b->y;
    for (int32_t index = 0; index < 4; ++index) {
        const PharaohGatehouseImagePart *part = &c->commit->gatehouse_parts[index];
        const uint8_t angle = b->industry_orientation;
        int32_t part_x = x, part_y = y;
        if (angle == 0) { part_x = add(x, part->dx); part_y = add(y, part->dy); }
        else if (angle == 1) { part_x = add(x, part->dy); part_y = add(y, part->dx); }
        *c->load->building_tiles->rows = (uint32_t)part->size;
        *c->load->building_tiles->columns = (uint32_t)part->size;
        pharaoh_map_clear_building_tiles(c->load->building_tiles, building,
            part_x, part_y);
    }
}

/* Exact immutable5DF970 table, indexed by the retained full direction.
   Odd rows remain present even though the Native producer uses0/2/4/6. */
static const int32_t temple_part_offsets[8][2] = {
    {3, 0}, {3, 0}, {0, -3}, {3, 0},
    {-3, 0}, {3, 0}, {0, 3}, {3, 0}
};

static uint32_t temple_group(const PharaohCityUndoLiveContext *c,
    int32_t direction, int32_t part)
{
    const int32_t view = *c->load->surface->orientation;
    const uint32_t index = ((uint32_t)(direction / 2) * 4u +
        (uint32_t)(view / 2)) * 3u + (uint32_t)part;
    return (uint32_t)c->load->temple_images->tables->temple_groups[index];
}

void pharaoh_city_undo_temple_complex_create(const PharaohCityUndoLiveContext *c,
    int32_t building, int32_t x, int32_t y, uint32_t owner, int32_t kind,
    uint32_t initialize)
{
    PharaohMapSurfaceContext *surface = c->load->surface;
    PharaohBuildingTypeRecord *buildings = surface->buildings;
    PharaohBuildingTypeRecord *b = &buildings[building];
    if (b->runtime_value_038 != 0) return;
    int32_t direction;
    if (initialize != 0) {
        const uint32_t player = *c->load->current_player;
        b->temple_upgrade = 0;
        c->support->monuments->players[player].temple_complex_enabled = 1;
        *c->player_extras[player].temple_complex_upgrades = 2;
        c->load->cities[player].temple_complex_building_id = building;
        direction = signed_bits(*c->load->allocation->rotation * 2u -
            (uint32_t)*surface->orientation);
        if (direction < 0) direction = add(direction, 8);
        b->industry_orientation = (uint8_t)direction;
    } else direction = b->industry_orientation;
    int32_t current = building;
    for (int32_t part = 0; part < 3; ++part) {
        uint32_t group;
        if (part == 0) {
            group = temple_group(c, direction, 0);
            if (initialize != 0) b->runtime_value_038 = 0;
        } else {
            const int32_t previous = part == 1 ? building :
                buildings[building].next_part_building_id;
            if (initialize != 0) {
                const int32_t px = signed_bits((uint32_t)x +
                    (uint32_t)temple_part_offsets[direction][0] * (uint32_t)part);
                const int32_t py = signed_bits((uint32_t)y +
                    (uint32_t)temple_part_offsets[direction][1] * (uint32_t)part);
                current = pharaoh_building_allocate(c->load->allocation, owner,
                    (uint32_t)kind, px, py, 0, 0);
                b = &buildings[current];
                b->industry_orientation = (uint8_t)direction;
                (void)pharaoh_map_building_undo_capture(c->capture, current);
                b->runtime_value_038 = (int16_t)previous;
                buildings[previous].next_part_building_id = (int16_t)current;
                b->next_part_building_id = 0;
            }
            group = part == 1 ? 570u : temple_group(c, direction, part);
        }
        if (initialize != 0) {
            if (direction == 0 || direction == 4) b->subtype = (int16_t)part;
            else if (direction == 2 || direction == 6)
                b->subtype = (int16_t)(part + 3);
        }
        const int32_t view = *surface->orientation;
        const uint32_t lateral = direction == 2 || direction == 6;
        const uint32_t bias = ((view == 0 || view == 4) ? lateral : !lateral) ? 3u : 0u;
        const uint32_t image = surface->groups[group].first_image + bias;
        const int32_t px = signed_bits((uint32_t)x +
            (uint32_t)temple_part_offsets[direction][0] * (uint32_t)part);
        const int32_t py = signed_bits((uint32_t)y +
            (uint32_t)temple_part_offsets[direction][1] * (uint32_t)part);
        /* initialize0 deliberately retains the root ID for all three
           rectangles. Native does not advance the linked part here. */
        pharaoh_map_place_surface_square(surface, (uint16_t)current, px, py,
            3, 3, image, 8);
    }
    pharaoh_temple_complex_update_decoration(c->load->temple_images,
        pharaoh_building_follow_link(buildings, current));
    const uint32_t god = pharaoh_building_group_index(buildings, current);
    *c->player_extras[*c->load->current_player].temple_god_bonus = god == 4 ? 10u : 0u;
}

static void temple_create_provider(void *user, int32_t building,
    int32_t x, int32_t y, uint32_t owner, int32_t kind, uint32_t initialize)
{
    pharaoh_city_undo_temple_complex_create(user, building, x, y,
        owner, kind, initialize);
}

static void restore_temple_to_map(const PharaohCityUndoLiveContext *c,
    int32_t building, int32_t kind, int32_t x, int32_t y, int32_t size)
{
    /* The selected65..69 branch of canonical470990 loads its real sprite
       library and invokes only temple_complex_create. This local context
       view keeps all actual pools/providers and supplies that concrete C
       child without changing shared operation ownership or user_data. */
    PharaohConstructionLoadOperations operations = *c->load->operations;
    PharaohConstructionLoadContext load = *c->load;
    operations.user_data = (void *)c;
    operations.temple_complex_create = temple_create_provider;
    load.operations = &operations;
    pharaoh_building_construction_add_to_map(&load, *load.current_player,
        kind, building, x, y, size, 0, 0);
}

void pharaoh_city_undo_restore_building(const PharaohCityUndoLiveContext *c,
    int32_t building)
{
    if (building <= 0) return;
    PharaohBuildingTypeRecord *buildings = c->load->surface->buildings;
    PharaohBuildingTypeRecord *b = &buildings[building];
    int32_t size;
    /* Native property base5D06DC, stride20: its first DWORD is the size.
       image_group is a distinct field eight bytes farther into the row. */
    memcpy(&size, c->load->properties[b->building_type].native_00, sizeof(size));
    if ((uint8_t)pharaoh_building_is_farm(buildings, building) != 0)
        pharaoh_city_undo_place_farm(c->farms, building, 0);
    else if (b->building_type >= 65 && b->building_type <= 69)
        restore_temple_to_map(c, building, b->building_type, b->x, b->y, size);
    else if (b->building_type == 202)
        pharaoh_building_construction_add_to_map(c->load, *c->load->current_player,
            b->building_type, building, b->x, b->y, size, 0, 0);
    else pharaoh_map_place_surface_square(c->load->surface, (uint16_t)building,
        b->x, b->y, size, size, 0, 0);
    if (b->building_type == 76 || b->building_type == 181 || b->building_type == 182)
        b->runtime_value_b4 = 0;
    if (b->house_size != 0) b->house_population = 0;
    if (storage_kind(b->building_type)) {
        const uint8_t slot = b->storage_id;
        if (slot == 0 || c->support->storages[slot].in_use != 0)
            b->storage_id = (uint8_t)pharaoh_construction_storage_allocate(c->support,
                building);
        else {
            c->support->storages[slot].in_use = 1;
            if (*c->support->highest_storage <= slot) *c->support->highest_storage = slot;
        }
    }
    if (b->building_type == 136) {
        const uint8_t slot = b->runtime_value_ca;
        if (slot == 0 || c->support->ferry_queues[slot].queue.state != 0)
            b->runtime_value_ca = (uint8_t)pharaoh_construction_ferry_queue_allocate(
                c->support, (uint32_t)building);
        else c->support->ferry_queues[slot].queue.state = 1;
    }
    if (b->building_type == 74) {
        const uint8_t slot = b->repair_queue_id;
        PharaohBuildingAllocationContext *allocation = c->load->allocation;
        if (slot == 0 || allocation->repair_queue[slot - 1].active != 0)
            pharaoh_building_repair_queue_add(buildings, building,
                allocation->repair_queue, allocation->output_debug,
                allocation->debug_user);
        else {
            /* Native4859BF sets EBX=1 before485AAA stores BL. The reused
               legacy RestoreRepairSlot comment incorrectly claimed zero. */
            allocation->repair_queue[slot - 1].active = 1;
        }
    }
    const uint32_t player = *c->load->current_player;
    PharaohConstructionCity *city = &c->load->cities[player];
    const PharaohCityUndoPlayerExtras *extras = &c->player_extras[player];
    switch (b->building_type) {
        case 209:
            b->state = 1;
            *extras->festival_x = b->x;
            *extras->festival_y = b->y;
            city->festival_square_placed = 1;
            return;
        case 187: case 188: case 189:
            city->palace_grid_offset = b->grid_offset;
            b->state = 1;
            city->palace_x = b->x;
            city->palace_y = b->y;
            city->palace_placed = 1;
            return;
        case 77: case 78: case 79:
            b->state = 1;
            city->mansion_placed = 1;
            return;
        case 95:
            city->recruiter_grid_offset = b->grid_offset;
            b->state = 1;
            city->recruiter_x = b->x;
            city->recruiter_y = b->y;
            *extras->recruiter_placed = 1;
            return;
        case 94:
            b->state = 1;
            city->military_academy_placed = 1;
            return;
        case 50:
            city->distribution_center_grid_offset = b->grid_offset;
            b->state = 1;
            city->distribution_center_x = b->x;
            city->distribution_center_y = b->y;
            city->distribution_center_placed = 1;
            return;
        case 136:
            if (b->linked_ferry_building_id > 0)
                buildings[b->linked_ferry_building_id].linked_ferry_building_id = building;
            b->state = 1;
            return;
        case 210:
            city->sphinx_stage = *extras->sphinx_stage_before_delete;
            (void)pharaoh_construction_monument_register(c->support, building);
            b->state = 1;
            return;
        case 182:
            b->state = 1;
            *c->load->warship_wharf_count += 1u;
            return;
        case 183: case 213: case 222: case 207: case 208:
            (void)pharaoh_construction_monument_register(c->support, building);
            b->state = 1;
            return;
        default: break;
    }
    if (b->building_type >= 65 && b->building_type <= 69) {
        c->support->monuments->players[player].temple_complex_enabled = 1;
        *extras->temple_complex_upgrades = 2;
        city->temple_complex_building_id = building;
    }
    b->state = 1;
}

typedef struct UndoMonumentTileAdapter {
    const PharaohCityUndoLiveContext *live;
    PharaohMonumentTileContext *method;
    int32_t building;
} UndoMonumentTileAdapter;

/* Concrete adapters to the existing47C480 and477A40 implementations.
   The normalized method sees one selected record; its relative identity is
   translated back to the actual Native building for every child call. */
static void monument_remove_tile(void *user, int32_t relative_building,
    int32_t x, int32_t y)
{
    UndoMonumentTileAdapter *a = user;
    PharaohMapBuildingTilesContext *tiles = a->live->load->building_tiles;
    (void)relative_building;
    *tiles->rows = (uint32_t)a->method->tile_size_x;
    *tiles->columns = (uint32_t)a->method->tile_size_y;
    pharaoh_map_clear_building_tiles(tiles, a->building, x, y);
    a->method->tile_size_x = signed_bits(*tiles->rows);
    a->method->tile_size_y = signed_bits(*tiles->columns);
}

static void monument_set_terrain(void *user, int32_t x, int32_t y,
    uint32_t terrain)
{
    UndoMonumentTileAdapter *a = user;
    pharaoh_editor_terrain_set_rock_kind(a->live->terrain_kind, x, y, terrain);
}

static void monument_remove_actual(const PharaohCityUndoLiveContext *c,
    int32_t building, enum PharaohRotatedMonument kind)
{
    const PharaohBuildingTypeRecord *b = &c->load->surface->buildings[building];
    const PharaohMonumentImagePart *part =
        &c->load->monument_images[kind]->parts[b->subtype];
    const PharaohMonumentPartDesc selected_part = { part->size, part->kind };
    PharaohMonumentBuilding selected_building = {0};
    selected_building.x = b->x;
    selected_building.y = b->y;
    selected_building.subtype = 0;
    PharaohMonumentTileContext method = {0};
    method.buildings = &selected_building;
    method.open_ground_kind = 9;
    method.lighthouse_tower_kind = 0;
    method.monument_rock_terrain = UINT32_C(0x20000002);
    method.rock_terrain = 2;
    UndoMonumentTileAdapter adapter = {c, &method, building};
    method.remove_tile = monument_remove_tile;
    method.set_terrain = monument_set_terrain;
    method.user_data = &adapter;
    switch (kind) {
        case PHARAOH_ROTATED_LIBRARY:
            pharaoh_library_remove_tiles(&method, &selected_part, 0); break;
        case PHARAOH_ROTATED_CAESAREUM:
            pharaoh_caesareum_remove_tiles(&method, &selected_part, 0); break;
        case PHARAOH_ROTATED_LIGHTHOUSE:
            pharaoh_lighthouse_remove_tiles(&method, &selected_part, 0); break;
        case PHARAOH_ROTATED_ABU_SIMBEL:
            pharaoh_abu_simbel_remove_tiles(&method, &selected_part, 0); break;
        default: break;
    }
}

static void royal_tomb_clear_actual(const PharaohCityUndoLiveContext *c,
    int32_t building, enum PharaohRotatedMonument kind)
{
    PharaohMapSurfaceContext *surface = c->load->surface;
    const PharaohBuildingTypeRecord *b = &surface->buildings[building];
    const PharaohMonumentImageContext *image = c->load->monument_images[kind];
    const PharaohMonumentImagePart *part = &image->parts[b->subtype];
    const int32_t size = part->size, x = b->x, y = b->y;
    const int32_t selected_last = b->subtype == *image->last_part ? 0 : 1;
    const PharaohMonumentPartDesc one_cell = {1, part->kind};
    PharaohMonumentBuilding selected_building = {0};
    /*546660 has no children. Its normalized canonical owner uses a widened
      property cell and omits the Native origin. A one-cell projection lets
      that same algorithm run with the actual origin and BYTE/WORD stores;
      only the selected part/last-part equality is relocated to0/1. */
    for (int32_t dx = 0; dx < size; ++dx) {
        for (int32_t dy = 0; dy < size; ++dy) {
            const uint32_t tile = surface->map->origin +
                (uint32_t)add(x, dx) + (uint32_t)add(y, dy) * 228u;
            uint16_t building_id = 0, sprite = 0;
            uint32_t terrain = 0, bitfields = surface->property_flags[tile];
            uint8_t canals = 0, damage = 0, edge = surface->edge_flags[tile];
            PharaohMonumentTileContext method = {0};
            method.buildings = &selected_building;
            method.grid_width = 1;
            method.monument_rock_terrain = UINT32_C(0x20000002);
            method.groundwater_terrain = UINT32_C(0x80);
            method.destroyed_bitfield_keep = UINT32_C(0xe0);
            method.destroyed_edge_keep = 0xc0;
            method.edge_draw_tile = 0x40;
            method.building_ids = &building_id;
            method.terrain = &terrain;
            method.bitfields = &bitfields;
            method.canals = &canals;
            method.building_damage = &damage;
            method.edge = &edge;
            method.sprite = &sprite;
            switch (kind) {
                case PHARAOH_ROTATED_SMALL_TOMB:
                    method.small_tomb_descs = &one_cell;
                    method.small_last_part = selected_last;
                    pharaoh_small_royal_tomb_clear_tiles(&method, 0); break;
                case PHARAOH_ROTATED_MEDIUM_TOMB:
                    method.medium_tomb_descs = &one_cell;
                    method.medium_last_part = selected_last;
                    pharaoh_medium_royal_tomb_clear_tiles(&method, 0); break;
                case PHARAOH_ROTATED_LARGE_TOMB:
                    method.large_tomb_descs = &one_cell;
                    method.large_last_part = selected_last;
                    pharaoh_large_royal_tomb_clear_tiles(&method, 0); break;
                case PHARAOH_ROTATED_GRAND_TOMB:
                    method.grand_tomb_descs = &one_cell;
                    method.grand_last_part = selected_last;
                    pharaoh_grand_royal_tomb_clear_tiles(&method, 0); break;
                default: break;
            }
            /* Store order and widths from pinned546660, including the
               intermediate terrain clear and distinct WORD sprite grid. */
            surface->building_ids[tile] = (int16_t)building_id;
            surface->map->terrain[tile] = 0;
            surface->property_flags[tile] = (uint8_t)bitfields;
            surface->canal_water[tile] = canals;
            c->load->building_tiles->native_a9cc1c[tile] = sprite;
            surface->edge_flags[tile] = edge;
            c->load->building_tiles->native_adc36c[tile] = damage;
            surface->map->terrain[tile] = terrain;
        }
    }
}

static void undo_fort(const PharaohCityUndoLiveContext *c, int32_t building)
{
    const PharaohBuildingTypeRecord *b = &c->load->surface->buildings[building];
    for (int32_t id = 1; id < 50; ++id) {
        const PharaohCityUndoFormation *formation = &c->formations[id];
        if (formation->state == 0 || formation->is_legion != 1 ||
            formation->building_id != building) continue;
        int32_t kind;
        switch (b->subtype) {
            case 13: kind = 2; break;
            case 12: kind = 0; break;
            case 11: kind = 1; break;
            default: return;
        }
        const PharaohConstructionSavedLegion *finished = &c->finished_legions[kind];
        if (formation->legion_id == finished->legion_id.bytes.low)
            memcpy(&c->started_legions[kind], finished, sizeof(*finished));
        return;
    }
}

static void remove_created_tiles(const PharaohCityUndoLiveContext *c,
    int32_t building)
{
    PharaohBuildingTypeRecord *buildings = c->load->surface->buildings;
    const PharaohBuildingTypeRecord *b = &buildings[building];
    /* Native4854C0..485621 reloads the type after each actual child. */
    if (b->building_type == 183 && b->subtype == 0)
        pharaoh_pyramid_clear_complex_temple(c->pyramid, building);
    if (b->building_type == 136) {
        if (b->linked_ferry_building_id > 0)
            buildings[b->linked_ferry_building_id].linked_ferry_building_id = -1;
        pharaoh_ferry_queues_release(c->ferry, building);
    }
    if (b->building_type == 31) pharaoh_city_undo_venue_remove(c, building, 31);
    if (b->building_type == 30) pharaoh_city_undo_venue_remove(c, building, 30);
    if (b->building_type == 33) pharaoh_city_undo_venue_remove(c, building, 33);
    if (b->building_type == 171) pharaoh_city_undo_gatehouse_remove(c, building);
    if (b->building_type == 202) pharaoh_city_undo_gatehouse_remove(c, building);
    if (b->building_type == 225)
        monument_remove_actual(c, building, PHARAOH_ROTATED_LIBRARY);
    if (b->building_type == 230)
        monument_remove_actual(c, building, PHARAOH_ROTATED_ABU_SIMBEL);
    if (b->building_type == 227)
        monument_remove_actual(c, building, PHARAOH_ROTATED_CAESAREUM);
    if (b->building_type == 228)
        monument_remove_actual(c, building, PHARAOH_ROTATED_LIGHTHOUSE);
    if (b->building_type == 229)
        royal_tomb_clear_actual(c, building, PHARAOH_ROTATED_SMALL_TOMB);
    if (b->building_type == 234)
        royal_tomb_clear_actual(c, building, PHARAOH_ROTATED_MEDIUM_TOMB);
    if (b->building_type == 235)
        royal_tomb_clear_actual(c, building, PHARAOH_ROTATED_LARGE_TOMB);
    if (b->building_type == 236)
        royal_tomb_clear_actual(c, building, PHARAOH_ROTATED_GRAND_TOMB);
    if (b->building_type == 57) undo_fort(c, building);
}

void pharaoh_city_undo_perform(const PharaohCityUndoLiveContext *c)
{
    const PharaohCityUndoState *state = c->state;
    if (*state->ready != 1 || *state->available != 1) return;
    const uint32_t cost = *state->cost, player = *c->load->current_player;
    PharaohConstructionCommitCity *city = &c->commit->cities[player];
    city->treasury = add(city->treasury, signed_bits(cost));
    *state->available = 0;
    city->construction_current_year -= cost;
    for (int32_t resource = 0; resource < 36; ++resource)
        if (state->resources[resource] != 0)
            (void)pharaoh_city_undo_store_anywhere(c->storage, resource,
                signed_bits(state->resources[resource]), 0);
    const int32_t tool = *state->saved_tool;
    const PharaohCityUndoMaps *maps = c->maps;
    PharaohMapSurfaceContext *surface = c->load->surface;
    PharaohBuildingTypeRecord *buildings = surface->buildings;
    const size_t dword_bytes = PHARAOH_CITY_UNDO_GRID_CELLS * sizeof(uint32_t);
    if (tool == 9) {
        int32_t mausoleum = 0;
        for (int32_t i = 0; i < signed_bits(*state->count); ++i) {
            const int32_t building = state->building_ids[i];
            if (building == 0) continue;
            memcpy(&buildings[building], &state->building_backup[i],
                sizeof(PharaohBuildingTypeRecord));
            pharaoh_city_undo_restore_building(c, building);
            if (buildings[building].building_type == 222) mausoleum = building;
        }
        /* These are the existing Native undo backups, not new snapshots. */
        memcpy(maps->terrain, maps->saved_terrain, dword_bytes);
        memcpy(maps->canals, maps->saved_canals, PHARAOH_CITY_UNDO_GRID_CELLS);
        memcpy(maps->sprites, maps->saved_sprites, PHARAOH_CITY_UNDO_GRID_CELLS);
        memcpy(maps->images, maps->saved_images, dword_bytes);
        memcpy(maps->bitfields, maps->saved_bitfields, PHARAOH_CITY_UNDO_GRID_CELLS);
        memcpy(maps->edges, maps->saved_edges, PHARAOH_CITY_UNDO_GRID_CELLS);
        for (uint32_t i = 0; i < PHARAOH_CITY_UNDO_GRID_CELLS; ++i)
            maps->bitfields[i] &= 0xafu;
        pharaoh_city_undo_restore_houses(c->load->allocation, c->load->building_tiles);
        if (mausoleum != 0) pharaoh_city_undo_mausoleum_images(c->mausoleum, mausoleum);
    } else if (tool == 8 || tool == 5 || tool == 6 || tool == 168 || tool == 169) {
        memcpy(maps->terrain, maps->saved_terrain, dword_bytes);
        memcpy(maps->canals, maps->saved_canals, PHARAOH_CITY_UNDO_GRID_CELLS);
        pharaoh_map_restore_free_tile_images(surface, maps->saved_images);
    } else if (tool == 82 || tool == 83) {
        memcpy(maps->terrain, maps->saved_terrain, dword_bytes);
        memcpy(maps->sprites, maps->saved_sprites, PHARAOH_CITY_UNDO_GRID_CELLS);
        pharaoh_map_restore_free_tile_images(surface, maps->saved_images);
    } else if (tool == 38 || tool == 39) {
        memcpy(maps->terrain, maps->saved_terrain, dword_bytes);
        memcpy(maps->canals, maps->saved_canals, PHARAOH_CITY_UNDO_GRID_CELLS);
        memcpy(maps->bitfields, maps->saved_bitfields, PHARAOH_CITY_UNDO_GRID_CELLS);
        memcpy(maps->edges, maps->saved_edges, PHARAOH_CITY_UNDO_GRID_CELLS);
        pharaoh_map_restore_free_tile_images(surface, maps->saved_images);
    } else if (*state->count != 0) {
        if (tool == 7) {
            memcpy(maps->terrain, maps->saved_terrain, dword_bytes);
            memcpy(maps->canals, maps->saved_canals, PHARAOH_CITY_UNDO_GRID_CELLS);
            pharaoh_map_restore_free_tile_images(surface, maps->saved_images);
        }
        for (int32_t i = 0; i < signed_bits(*state->count); ++i) {
            const int32_t building = state->building_ids[i];
            if (building == 0) continue;
            remove_created_tiles(c, building);
            buildings[building].state = 2;
        }
    }
    pharaoh_map_routing_initialize_citizen(c->commit->routing);
    pharaoh_map_routing_initialize_noncitizen(c->commit->routing);
    pharaoh_map_routing_initialize_walls(c->commit->routing);
    *state->count = 0;
    pharaoh_city_undo_storage_reset_links(c->maintenance);
    pharaoh_city_undo_ferry_reset_links(c->maintenance);
    pharaoh_city_undo_repair_reset_links(c->maintenance);
    pharaoh_monument_rebuild_player_slots(c->support->monuments);
    pharaoh_map_refresh_all_tiles(surface);
}

void pharaoh_city_sidebar_undo(const PharaohCityUndoLiveContext *c)
{
    pharaoh_city_undo_perform(c);
    *c->redraw = 1;
}
