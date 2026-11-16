#include "pharaoh/military/enemy_queries.h"

#include <string.h>

enum {
    WALKER_ALIVE = 1, WALKER_ARCHER = 11, WALKER_CHARIOTEER = 13,
    ENEMY_ARCHER = 43, ENEMY_HEAVY2 = 53, ENEMY_EGYPT_INFANTRY = 54,
    ENEMY_EGYPT_ARCHER = 55, ENEMY_EGYPT_CHARIOT = 56,
    KINGDOM_SOLDIER = 57, BEDOUIN = 99, ACTION_CORPSE = 4,
    ACTION_ATTACK = 5, ACTION_ENEMY_INITIAL = 9, MAX_FORMATION_WALKERS = 16,
    MAX_LEGIONS = 6, STRENGTH_RINGS = 6, MAX_STRENGTH_RADIUS = 7,
    SOLDIERS_PER_RING = 3, ATTACK_RANDOM = 4, ATTACK_LIST_LENGTH = 100,
    BUILDING_IN_USE = 1, STORAGE_YARD = 72, STORAGE_YARD_UP = 193,
    MESSAGE_ENEMIES_LEAVING = 304
};

static int32_t maximum_distance(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    int32_t dx = x1 > x2 ? x1 - x2 : x2 - x1;
    int32_t dy = y1 > y2 ? y1 - y2 : y2 - y1;
    return dx > dy ? dx : dy;
}

void pharaoh_set_enemies_fleeing(PharaohEnemyQueryContext *c)
{
    int32_t left = c->enemies_to_leave, first = 0;
    size_t id;
    if (left <= 0) return;
    for (id = 1; id < c->walker_count && left > 0; ++id) {
        PharaohMilitaryWalker *w = &c->walkers[id];
        int32_t type = w->type;
        if (w->state != WALKER_ALIVE) continue;
        if ((type >= ENEMY_ARCHER && type <= ENEMY_HEAVY2) ||
            type == KINGDOM_SOLDIER || type == BEDOUIN ||
            type == ENEMY_EGYPT_INFANTRY || type == ENEMY_EGYPT_ARCHER ||
            type == ENEMY_EGYPT_CHARIOT) {
            --left; w->action_state = ACTION_CORPSE;
            if (first == 0) first = w->grid_offset;
        }
    }
    c->enemies_to_leave = 0; c->message_use_popup = true;
    c->post_message(c->user_data, MESSAGE_ENEMIES_LEAVING, first);
}

void pharaoh_reset_enemy_walkers(PharaohEnemyQueryContext *c, int32_t formation)
{
    int32_t i;
    for (i = 0; i < MAX_FORMATION_WALKERS; ++i) {
        int32_t id = c->formations[formation].walkers[i];
        if (id > 0) {
            PharaohMilitaryWalker *w = &c->walkers[id];
            if (w->action_state != ACTION_ATTACK && w->action_state != ACTION_CORPSE) {
                w->action_state = ACTION_ENEMY_INITIAL; w->wait_ticks = 0;
            }
        }
    }
}

void pharaoh_add_soldier_strength(PharaohEnemyQueryContext *c,
    int32_t x, int32_t y, int32_t radius, int32_t amount)
{
    int32_t xmin = x - radius, ymin = y - radius, xmax = x + radius,
        ymax = y + radius, xx, yy;
    if (xmin < 0) xmin = 0; if (ymin < 0) ymin = 0;
    if (xmax >= c->grid_width) xmax = c->grid_width - 1;
    if (ymax >= c->grid_height) ymax = c->grid_height - 1;
    c->map_result_y = ymin; if (ymin > ymax) return; c->map_result_y = ymax + 1;
    for (yy = ymin; yy <= ymax; ++yy) {
        int32_t offset = c->grid_start + yy * c->grid_stride + xmin;
        c->map_result_x = xmin; if (xmin > xmax) continue; c->map_result_x = xmax + 1;
        for (xx = xmin; xx <= xmax; ++xx, ++offset) {
            uint8_t value = (uint8_t)(c->soldier_strength[offset] + amount);
            int32_t walker = c->walker_ids[offset];
            c->soldier_strength[offset] = value;
            if (walker > 0 && c->walkers[walker].type >= WALKER_ARCHER &&
                c->walkers[walker].type <= WALKER_CHARIOTEER)
                c->soldier_strength[offset] = (uint8_t)(value + 2);
        }
    }
}

void pharaoh_update_soldier_strength(PharaohEnemyQueryContext *c)
{
    int32_t id, ring;
    memset(c->soldier_strength, 0, c->grid_cell_count);
    for (id = 1; id <= MAX_LEGIONS && (size_t)id < c->formation_count; ++id) {
        PharaohFormation *f = &c->formations[id];
        if (f->in_use != 1u || f->is_legion == 0u) continue;
        for (ring = 0; ring < STRENGTH_RINGS; ++ring)
            if (f->num_walkers > ring * SOLDIERS_PER_RING)
                pharaoh_add_soldier_strength(c, f->x, f->y,
                    MAX_STRENGTH_RADIUS - ring, 1);
    }
}

int32_t pharaoh_get_maximum_soldier_strength(PharaohEnemyQueryContext *c,
    int32_t x, int32_t y, int32_t radius)
{
    int32_t xmin = x - radius, ymin = y - radius, xmax = x + radius,
        ymax = y + radius, best = 0, bestx = x, besty = y, xx, yy;
    if (xmin < 0) xmin = 0; if (ymin < 0) ymin = 0;
    if (xmax >= c->grid_width) xmax = c->grid_width - 1;
    if (ymax >= c->grid_height) ymax = c->grid_height - 1;
    c->map_result_y = ymin; if (ymin > ymax) return 0;
    for (yy = ymin; yy <= ymax; ++yy) {
        int32_t offset = c->grid_start + yy * c->grid_stride + xmin;
        c->map_result_x = xmin;
        for (xx = xmin; xx <= xmax; ++xx, ++offset)
            if (c->routing_distance[offset] > 0 && c->soldier_strength[offset] > best) {
                best = c->soldier_strength[offset]; bestx = xx; besty = yy;
            }
        c->map_result_x = xx;
    }
    c->map_result_y = yy; if (best <= 0) return 0;
    c->map_result_x = bestx; c->map_result_y = besty; return 1;
}

static void find_target(PharaohEnemyQueryContext *c, int32_t formation,
    const int16_t *list, int32_t *best_index, int32_t *best_distance,
    int32_t *best_id)
{
    size_t id;
    PharaohFormation *enemies = &c->formations[formation];
    for (id = 1; id < c->building_count; ++id) {
        PharaohMilitaryBuilding *b = &c->buildings[id];
        int32_t i;
        if (b->state != BUILDING_IN_USE || c->soldier_strength[b->grid_offset] != 0u ||
            c->routing_distance[b->grid_offset] <= 0) continue;
        for (i = 0; i <= *best_index && i < ATTACK_LIST_LENGTH; ++i) {
            int32_t distance;
            if (list[i] == 0) break; if (b->type != list[i]) continue;
            distance = maximum_distance(enemies->x, enemies->y, b->x, b->y);
            if (i < *best_index || distance < *best_distance) {
                *best_index = i; *best_distance = distance; *best_id = (int32_t)id;
            }
            break;
        }
    }
}

bool pharaoh_set_enemy_target_building(PharaohEnemyQueryContext *c,
    int32_t formation)
{
    PharaohFormation *enemy = &c->formations[formation];
    int32_t kind = enemy->attack_type, best_index = ATTACK_LIST_LENGTH,
        best_id = 0, best_distance = 10000;
    PharaohMilitaryBuilding *b;
    if (kind == ATTACK_RANDOM) kind = (int32_t)(c->random_value(c->user_data) & 3u);
    c->build_enemy_routing(c->user_data, enemy->x, enemy->y);
    find_target(c, formation, c->attack_priority + kind * ATTACK_LIST_LENGTH,
        &best_index, &best_distance, &best_id);
    if (best_id <= 0) {
        find_target(c, formation, c->attack_fallback,
            &best_index, &best_distance, &best_id);
        if (best_id <= 0) return false;
    }
    b = &c->buildings[best_id];
    if (b->type == STORAGE_YARD || b->type == STORAGE_YARD_UP) {
        enemy->destination_x = (int16_t)(b->x + 1);
        enemy->destination_building_id = (int16_t)(best_id + 1);
    } else {
        enemy->destination_x = b->x;
        enemy->destination_building_id = (int16_t)best_id;
    }
    enemy->destination_y = b->y; return true;
}
