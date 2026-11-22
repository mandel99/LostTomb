#include "pharaoh/scenario/scene_counts.h"
#include "pharaoh/scenario/scene_industry.h"
#include <string.h>

_Static_assert(offsetof(PharaohBuildingTypeRecord, immigrant_walker_id) == 0x2e &&
    offsetof(PharaohBuildingTypeRecord, runtime_value_a6) == 0xa6 &&
    offsetof(PharaohBuildingTypeRecord, temple_upgrade) == 0xa9 &&
    offsetof(PharaohBuildingTypeRecord, runtime_value_b4) == 0xb4 &&
    offsetof(PharaohWalkerQueryRecord, state) == 0x0e &&
    offsetof(PharaohWalkerQueryRecord, entertainment_building_id) == 0x5e,
    "native count cleanup fields");

/* Literal slots in the two36-DWORD arrays, including native holes. */
static int32_t service_slot(uint16_t kind)
{
    switch (kind) {
    case 0x66: return 1; case 0xc2: return 2; case 0x67: return 3;
    case 0x69: return 4; case 0x68: return 5; case 0xc4: return 6;
    case 0x73: return 8; case 0x70: return 10; case 0x6d: return 11;
    case 0xcc: return 12; case 0x72: return 13; case 0x64: return 14;
    case 0x6e: return 15; case 0x65: return 16; case 0xe0: return 32;
    case 0x6f: return 17; case 0xa2: return 18; case 0x71: return 19;
    case 0x6c: return 20; case 0xa1: return 21; case 0xc3: return 22;
    case 0xcb: return 23; case 0x6a: return 24; case 0x6b: return 25;
    case 0xd8: return 26; case 0xcd: return 28; case 0xd9: return 29;
    case 0xdd: return 30; case 0xe8: return 34; case 0xe9: return 33;
    default: return 0;
    }
}

/* Complete Unit00933 C reuse, native004677C0. Counters have native wrap widths. */
void pharaoh_city_rebuild_building_counts(PharaohSceneCounts *c)
{
    PharaohCityBuildingCounts *const n = c->counts;
    int32_t owner = *c->initialize->current_player;
    PharaohScenarioPlayerRuntime *p = &c->initialize->players[owner];
    memset(n->services_active, 0, sizeof(n->services_active));
    memset(n->services_total, 0, sizeof(n->services_total));
    p->native_value_4488 = 0;
    p->native_value_4630 = 0;
    memset(p->dock_building_ids, 0, sizeof(p->dock_building_ids));
    p->native_value_001c = 0;
    memset(n->total, 0, sizeof(n->total));
    memset(n->active, 0, sizeof(n->active));
    for (int32_t id = 1; id <= *c->highest_building; ++id) {
        PharaohBuildingTypeRecord *const b = &c->buildings[id];
        int32_t kind, slot, force_services = 0, linked;
        owner = *c->initialize->current_player;
        p = &c->initialize->players[owner];
        if (b->state != 1 && b->state != 3) continue;
        kind = b->building_type;
        if (b->house_size) {
            ++n->total[kind];
            if (b->house_population) ++n->active[kind];
            continue;
        }
        if (kind == 0x5a || kind == 0x5c || kind == 0xb4) {
            ++n->total[kind];
            if (b->farm_water_access && (kind == 0x5c || b->num_workers > 0))
                ++n->active[kind];
        } else if ((slot = service_slot((uint16_t)kind)) != 0) {
            ++n->services_total[slot];
            if (b->num_workers > 0) ++n->services_active[slot];
        } else if (kind == 0x4c) {
            ++n->services_total[7];
            if (b->num_workers > 0) {
                ++p->native_value_4488;
                if (b->runtime_value_b4 == 0) ++p->native_value_4630;
                ++n->services_active[7];
            }
        } else if (kind == 0x4b || kind == 0xc0) {
            const uint32_t dock = n->total[0x4b];
            if ((int32_t)dock < 10) {
                if (b->num_workers > 0 && b->farm_water_access)
                    p->dock_building_ids[dock] = (int16_t)id;
                n->total[0x4b] = dock + 1;
            }
        } else if ((kind >= 0x41 && kind <= 0x45) ||
                   (kind >= 0x9c && kind <= 0xa0)) {
            if (pharaoh_building_follow_link(c->buildings, id) == id) {
                kind = b->building_type;
                ++n->total[kind];
                if (b->num_workers > 0) ++n->active[kind];
            }
        } else if (kind >= 0x8c && kind <= 0x95) {
            ++n->total[kind];
            if (pharaoh_map_has_road_access_nearby(c->access, b->x, b->y))
                ++n->active[b->building_type];
        } else {
            int32_t root;
            ++n->total[kind];
            if (kind == 0x1f) {
                root = pharaoh_building_follow_link(c->buildings, id);
                if (root != id) continue;
                if (c->buildings[root].num_workers > 0) ++n->active[31];
                force_services = 1;
            } else if (kind == 0x1e) {
                root = pharaoh_building_follow_link(c->buildings, id);
                if (root != id) continue;
                root = pharaoh_building_follow_link(c->buildings, id);
                if (c->buildings[root].num_workers > 0) {
                    ++n->active[31]; ++n->active[30];
                }
                force_services = 1;
            } else if (kind == 0x21) {
                if (b->num_workers > 0) {
                    ++n->active[31]; ++n->active[30]; ++n->active[33];
                }
                force_services = 1;
            } else if (kind == 0x20) {
                if (b->num_workers > 0) ++n->active[32];
            } else if (kind == 0xe2) {
                if (b->num_workers > 0) ++n->active[226];
            } else {
                if (kind == 0x5f) p->native_value_44ac = (uint32_t)id;
                if (b->num_workers > 0) ++n->active[kind];
            }
        }
        linked = b->immigrant_walker_id;
        if (linked && (c->walkers[linked].state != 1 ||
            c->walkers[linked].entertainment_building_id != id))
            b->immigrant_walker_id = 0;
        if (force_services ||
            pharaoh_building_is_floodplain_farm(&c->farm_query, id)) {
            uint8_t count = 0;
            if (b->runtime_value_a7) { --b->runtime_value_a7; ++count; }
            if (b->runtime_value_a8) { --b->runtime_value_a8; ++count; }
            if (b->temple_upgrade) { --b->temple_upgrade; ++count; }
            b->runtime_value_a6 = count;
        }
    }
}

static bool is_farm(void *user_data, int32_t id)
{
    const PharaohSceneCounts *c = user_data;
    return (uint8_t)pharaoh_building_is_farm(c->buildings, id) != 0;
}

static bool has_floodplain(void *user_data, int32_t id)
{
    PharaohSceneCounts *c = user_data;
    const PharaohBuildingTypeRecord *b = &c->buildings[id];
    /* Same native894A0->89470->blocking-tile chain, actual city coordinates. */
    return (uint8_t)pharaoh_region_has_active_occupant(c->occupancy,
        b->x, b->y, 3, 3) != 0;
}

static void counts(void *user_data)
{
    PharaohSceneCounts *c = user_data;
    c->occupancy->grid_origin = (uint32_t)c->initialize->geometry->grid_start;
    c->occupancy->terrain = c->roads->terrain;
    c->occupancy->last_origin = c->roads->scan_tile;
    c->access->grid_start = c->initialize->geometry->grid_start;
    c->access->ranks = c->initialize->players[
        *c->initialize->current_player].largest_road_networks;
    pharaoh_city_rebuild_building_counts(c);
    c->roads->scan_tile = c->occupancy->last_origin;
}

static void roads(void *user_data)
{
    PharaohSceneCounts *c = user_data;
    pharaoh_city_rebuild_road_networks(c->roads);
}

void pharaoh_saved_game_bind_count_steps(
    PharaohSavedGameInitializeOperations *ops, PharaohSceneCounts *c)
{
    c->initialize->operations = ops;
    c->roads->geometry = c->initialize->geometry;
    c->roads->players = c->initialize->players;
    c->roads->current_player = c->initialize->current_player;
    c->access->buildings = c->buildings;
    c->access->terrain = c->roads->terrain;
    c->access->regions = c->roads->regions;
    c->farm_query.is_farm = is_farm;
    c->farm_query.is_on_floodplain = has_floodplain;
    c->farm_query.user_data = c;
    ops->step[PHARAOH_INIT_BUILDING_COUNTS] =
        (PharaohSavedGameStepBinding){c, counts};
    ops->step[PHARAOH_INIT_ROAD_NETWORK] =
        (PharaohSavedGameStepBinding){c, roads};
}
