#include "pharaoh/scenario/scene_routing.h"

#include <string.h>

enum { ROUTING_CELLS = 0xcb10, ROUTING_GRID_WIDTH = 228 };

/* Complete typed zero-callee C translation of Pharaoh.exe 0x00517690. */
void pharaoh_map_routing_update_water_rect(
    const uint32_t *terrain, const uint8_t *tile_direction,
    uint8_t *water_routing, int32_t map_origin, int32_t map_width,
    int32_t map_height, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    static const int32_t neighbors[8] = {
        -228, -227, 1, 229, 228, 227, -1, -229
    };
    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 >= map_width) x2 = map_width - 1;
    if (y2 >= map_height) y2 = map_height - 1;

    for (int32_t y = y1; y <= y2; ++y) {
        int32_t tile = map_origin + y * ROUTING_GRID_WIDTH + x1;
        for (int32_t x = x1; x <= x2; ++x, ++tile) {
            uint8_t value;
            const uint32_t flags = terrain[tile];
            if ((flags & 4u) == 0) {
                value = 0xfcu;
            } else if ((flags & 0x80000u) != 0) {
                value = 0xffu;
            } else {
                int32_t neighbor;
                for (neighbor = 0; neighbor < 8; ++neighbor)
                    if ((terrain[tile + neighbors[neighbor]] & 4u) == 0)
                        break;
                if (neighbor < 8) value = 0xffu;
                else if (y < 1 || x < 1 || y > map_height - 2 ||
                         x > map_width - 2) value = 0xfeu;
                else if (tile_direction[tile] == 13u) value = 0xffu;
                else if (tile_direction[tile] == 5u ||
                         tile_direction[tile] == 2u) value = 0xfdu;
                else value = 0;
            }
            water_routing[tile] = value;
        }
    }
}

static int32_t noncitizen_blocked_building_type(int32_t type)
{
    switch (type) {
        case 0x58: case 0x59: case 0x5d: case 0x63: case 0x88:
        case 0xb7: case 0xcf: case 0xd0: case 0xd2: case 0xd5:
        case 0xd7: case 0xde: case 0xe1: case 0xe3: case 0xe4:
        case 0xe5: case 0xe6: case 0xea: case 0xeb: case 0xec:
            return 1;
        default:
            return 0;
    }
}

static uint8_t noncitizen_routing_value(uint32_t flags,
    const int16_t *building_ids, const PharaohBuildingTypeRecord *buildings,
    int32_t tile)
{
    if ((flags & 0x80000u) != 0 || (flags & 0x10004u) == 0x10004u)
        return 0xffu;
    if ((flags & 0x8000u) != 0) return 4;
    if ((flags & 0x40u) != 0 || (flags & 0x40000000u) != 0) return 0;
    if ((flags & 0x20u) != 0) return 0;
    if ((flags & 0x400u) != 0) return 2;
    if ((flags & 8u) != 0) {
        const int32_t type = buildings[building_ids[tile]].building_type;
        if (type == 0x48) return 1;
        if (type == 0xca) return 4;
        if (noncitizen_blocked_building_type(type)) return 0xffu;
        if (type == 0x39) return 5;
        if (type == 0x36) return 0;
        return 1;
    }
    if ((flags & 0x100u) != 0) return 0;
    if ((flags & 0x4000u) != 0) return 3;
    if ((flags & 0x2040011u) != 0) return 0;
    if ((flags & 0x1000u) != 0) return 0;
    return (flags & 0xeefed77fu) != 0 ? 0xffu : 0;
}

/* Complete typed zero-callee C translation of Pharaoh.exe 0x005173A0. */
void pharaoh_map_routing_update_land_noncitizen_rect(
    const uint32_t *terrain, const int16_t *building_ids,
    const PharaohBuildingTypeRecord *buildings, uint8_t *routing,
    int32_t map_origin, int32_t map_width, int32_t map_height,
    int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 >= map_width) x2 = map_width - 1;
    if (y2 >= map_height) y2 = map_height - 1;
    for (int32_t y = y1; y <= y2; ++y) {
        int32_t tile = map_origin + y * ROUTING_GRID_WIDTH + x1;
        for (int32_t x = x1; x <= x2; ++x, ++tile)
            routing[tile] = noncitizen_routing_value(
                terrain[tile], building_ids, buildings, tile);
    }
}

/* Reused completed MapCommitConnect C; native00516F90. */
void pharaoh_map_routing_update_citizen_rect(PharaohSceneRouting *c,
    int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t update_links)
{
    const PharaohSavedGameGeometry *const g = c->geometry;
    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 >= g->width) x2 = g->width - 1;
    if (y2 >= g->height) y2 = g->height - 1;
    for (int32_t y = y1; y <= y2; ++y) {
        int32_t tile = g->grid_start + y * 228 + x1;
        for (int32_t x = x1; x <= x2; ++x, ++tile) {
            uint32_t flags = c->terrain[tile];
            uint8_t cost;
            if (update_links != 0 && (flags & 0x10044u) == 0x44u &&
                c->tile_direction[tile] == 0) {
                flags &= ~0x40u;
                c->terrain[tile] = flags;
            }
            if ((flags & 0x80000u) != 0 ||
                (flags & 0x10004u) == 0x10004u) cost = 0xff;
            else if ((flags & 0x40000040u) != 0) cost = 0;
            else if ((flags & 0x400u) != 0) cost = 2;
            else if ((flags & 0x10000000u) != 0) cost = 5;
            else if ((flags & 0x8008u) != 0) {
                const int32_t id = c->building_ids[tile];
                if (id == 0) {
                    c->terrain[tile] = flags & ~8u;
                    c->noncitizen[tile] = 4;
                    c->images[tile] = (c->random[tile] & 7u) +
                        *c->cleared_building_image;
                    c->edges[tile] = 0x40;
                    c->footprints[tile] &= 0xf0;
                    ++*c->buildings_without_id_cleared;
                    continue; /* Native leaves this citizen cell untouched. */
                }
                const PharaohBuildingTypeRecord *const b = &c->buildings[id];
                switch (b->building_type) {
                case 0x8a: case 0xd7: case 0x88: cost = 0; break;
                case 0x36: cost = 6; break;
                case 0xd1: cost = 4; break;
                case 0x5a:
                    cost = ((uint32_t)b->reserved_aa[0] -
                        (uint32_t)(*c->map_orientation / 2)) & 1u ?
                        0xfc : 0xfa;
                    break;
                default: cost = 0xff; break;
                }
            } else if ((flags & 0x100u) != 0) cost = 4;
            else if (c->flood_tiles[tile] != NULL &&
                     c->flood_tiles[tile]->is_floodplain == 0) {
                const uint32_t image = c->images[tile] - *c->flood_image_base;
                if ((int32_t)image < 48) cost = 4;
                else if (image == 52 || image == 48 || image == 54 || image == 50)
                    cost = 0xfb;
                else cost = 0xff;
            } else if ((flags & 0x2050011u) != 0) cost = 4;
            else if ((flags & 0x1000u) != 0) cost = 2;
            else cost = (flags & 0xeeffd77fu) != 0 ? 0xff : 4;
            c->citizen[tile] = cost;
        }
    }
    if (update_links != 0) {
        for (int32_t id = 0; id < PHARAOH_BUILDING_RECORD_COUNT; ++id) {
            const PharaohBuildingTypeRecord *const b = &c->buildings[id];
            if (b->building_type != 0x88 || (b->state != 1 && b->state != 3))
                continue;
            if (b->num_workers != 0 &&
                c->buildings[b->linked_ferry_building_id].num_workers > 0) {
                c->update_ferry_tiles(c->ferry_user_data, id, 1, 1);
            } else {
                c->update_ferry_tiles(c->ferry_user_data, id, 1, 3);
                c->detach_ferry(c->ferry_user_data, id);
            }
        }
    }
}

void pharaoh_map_routing_initialize_citizen(PharaohSceneRouting *c)
{
    memset(c->citizen, 0xff, ROUTING_CELLS);
    pharaoh_map_routing_update_citizen_rect(c, 0, 0,
        c->geometry->width - 1, c->geometry->height - 1, 1);
}

void pharaoh_map_routing_initialize_noncitizen(PharaohSceneRouting *c)
{
    const PharaohSavedGameGeometry *const g = c->geometry;
    memset(c->noncitizen, 0xff, ROUTING_CELLS);
    pharaoh_map_routing_update_land_noncitizen_rect(c->terrain, c->building_ids,
        c->buildings, c->noncitizen, g->grid_start, g->width, g->height,
        0, 0, g->width - 1, g->height - 1);
}

void pharaoh_map_routing_initialize_water(PharaohSceneRouting *c)
{
    const PharaohSavedGameGeometry *const g = c->geometry;
    memset(c->water, 0xff, ROUTING_CELLS);
    pharaoh_map_routing_update_water_rect(c->terrain, c->tile_direction,
        c->water, g->grid_start, g->width, g->height,
        0, 0, g->width - 1, g->height - 1);
}

/* Reused complete RestoreRebuildAccessMapResult algorithm; native005178E0.
 * The native consumers discard EAX, so the C contract is void. */
void pharaoh_map_routing_initialize_walls(PharaohSceneRouting *c)
{
    static const int32_t neighbors[4][3] = {
        {228, 229, 1}, {228, 227, -1}, {-228, -229, -1}, {-228, -227, 1}
    };
    const PharaohSavedGameGeometry *const g = c->geometry;
    memset(c->walls, 0xff, ROUTING_CELLS);
    const int32_t orientation = *c->city_orientation;
    int32_t tile = g->grid_start;
    for (int32_t y = 0; y < g->height; ++y) {
        for (int32_t x = 0; x < g->width; ++x, ++tile) {
            const uint32_t flags = c->terrain[tile];
            uint8_t value = 0xff;
            if ((flags & 0x4000u) != 0) {
                if (orientation == 0 || orientation == 2 ||
                    orientation == 4 || orientation == 6) {
                    int32_t count = 0;
                    for (int32_t n = 0; n < 3; ++n)
                        count += (c->terrain[tile + neighbors[orientation / 2][n]] &
                            0xc000u) != 0;
                    if (count == 3) value = 0;
                }
            } else if ((flags & 0x8000u) != 0) value = 0;
            c->walls[tile] = value;
        }
        tile += g->grid_border;
    }
}

static void citizen(void *c) { pharaoh_map_routing_initialize_citizen(c); }
static void noncitizen(void *c) { pharaoh_map_routing_initialize_noncitizen(c); }
static void water(void *c) { pharaoh_map_routing_initialize_water(c); }
static void walls(void *c) { pharaoh_map_routing_initialize_walls(c); }

void pharaoh_saved_game_bind_route_steps(
    PharaohSavedGameInitializeOperations *ops, PharaohSceneRouting *c)
{
    ops->step[PHARAOH_INIT_WATER_ROUTES] = (PharaohSavedGameStepBinding){c, water};
    ops->step[PHARAOH_INIT_CITIZEN_ROUTES] = (PharaohSavedGameStepBinding){c, citizen};
    ops->step[PHARAOH_INIT_NONCITIZEN_ROUTES] = (PharaohSavedGameStepBinding){c, noncitizen};
    ops->step[PHARAOH_INIT_WALL_ROUTES] = (PharaohSavedGameStepBinding){c, walls};
}
