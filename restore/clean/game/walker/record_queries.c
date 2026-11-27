#include "pharaoh/core/memory_fill.h"
#include "pharaoh/walker/record_queries.h"

void pharaoh_walker_save_type_78_sources(PharaohWalkerQueryRecord *walkers)
{
    int32_t id;
    for (id = 1; id < PHARAOH_WALKER_RECORD_COUNT; ++id) {
        PharaohWalkerQueryRecord *w = &walkers[id];
        if (w->type == 78 && w->state == 1 && w->action_state != 4) {
            const uint8_t *raw = (const uint8_t *)w;
            w->saved_source_x = *(const int16_t *)(raw + 0x14);
            w->saved_source_y = *(const int16_t *)(raw + 0x16);
        }
    }
}

void pharaoh_walker_detach_slot_link(
    PharaohWalkerQueryRecord *walkers, int16_t *slot_walkers,
    int32_t walker_id)
{
    const int16_t slot = walkers[walker_id].slot_link_id;
    if (slot > 0) {
        if (slot_walkers[slot] == walker_id) slot_walkers[slot] = 0;
        walkers[walker_id].slot_link_id = 0;
    }
}

/* Complete004EA9A0. Logical AL predicate. */
int32_t pharaoh_walker_type_is_herd_animal(int32_t walker_type)
{
    return walker_type >= 0x44 && walker_type <= 0x46;
}

void pharaoh_walker_recount_player_naval_types(
    const PharaohWalkerQueryRecord *walkers, uint32_t current_player,
    uint8_t *type_77_count, uint8_t *type_78_count,
    uint8_t *flagged_type_78_count)
{
    int32_t walker_id;
    *flagged_type_78_count = 0;
    *type_78_count = 0;
    *type_77_count = 0;
    for (walker_id = 1; walker_id < PHARAOH_WALKER_RECORD_COUNT;
         ++walker_id) {
        const PharaohWalkerQueryRecord *walker = &walkers[walker_id];
        if (walker->state == 0 || walker->owner != current_player) continue;
        if (walker->type == 78) {
            ++*type_78_count;
            if (((const uint8_t *)walker)[2] != 0) ++*flagged_type_78_count;
        } else if (walker->type == 77) {
            ++*type_77_count;
        }
    }
}

int32_t pharaoh_walker_find_linked_type_77(
    const PharaohWalkerQueryRecord *walkers, const int16_t *heads,
    int32_t head_index)
{
    int32_t walker_id = heads[head_index];
    while (walker_id != 0) {
        if (walkers[walker_id].type == 77) return walker_id;
        walker_id = walkers[walker_id].next_walker_id_on_same_tile;
    }
    return 0;
}

int32_t pharaoh_walker_find_linked_type_78(
    const PharaohWalkerQueryRecord *walkers, const int16_t *heads,
    int32_t head_index)
{
    int32_t walker_id = heads[head_index];
    while (walker_id != 0) {
        if (walkers[walker_id].type == 78) return walker_id;
        walker_id = walkers[walker_id].next_walker_id_on_same_tile;
    }
    return 0;
}

int32_t pharaoh_walker_enemy_near_building_site(
    const PharaohWalkerQueryRecord *walkers,
    int32_t first_x, int32_t first_y, int32_t second_x, int32_t second_y)
{
    int32_t walker_id;
    for (walker_id = 1; walker_id < PHARAOH_WALKER_RECORD_COUNT;
         ++walker_id) {
        const PharaohWalkerQueryRecord *walker = &walkers[walker_id];
        int32_t dx, dy;
        if (walker->state != 1 ||
            !((walker->type >= 0x2b && walker->type <= 0x39) ||
              walker->type == 99)) continue;
        dx = walker->x < first_x ? first_x - walker->x : walker->x - first_x;
        dy = walker->y < first_y ? first_y - walker->y : walker->y - first_y;
        if (dx <= 12 && dy <= 12) return 1;
        dx = walker->x < second_x ? second_x - walker->x : walker->x - second_x;
        dy = walker->y < second_y ? second_y - walker->y : walker->y - second_y;
        if (dx <= 12 && dy <= 12) return 1;
    }
    return 0;
}

int32_t pharaoh_minimap_walker_ship_kind(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id)
{
    while (walker_id != 0) {
        const uint8_t type = walkers[walker_id].type;
        if (type == 78 || type == 77 || type == 25 ||
            type == 76 || type == 20) return 1;
        if (type == 93 || type == 92 || type == 100 || type == 101) return 2;
        walker_id = walkers[walker_id].next_walker_id_on_same_tile;
    }
    return 0;
}

enum PharaohWalkerTextNameGroup {
    PHARAOH_NAME_CITIZEN = 254,
    PHARAOH_NAME_HOMELESS = 255,
    PHARAOH_NAME_LABORER = 256,
    PHARAOH_NAME_PATRICIAN = 257,
    PHARAOH_NAME_HUNTER = 258,
    PHARAOH_NAME_MARKET = 259,
    PHARAOH_NAME_DELIVERY_BOY = 260,
    PHARAOH_NAME_TRADE_SHIP = 261,
    PHARAOH_NAME_TRADE_CARAVAN = 262,
    PHARAOH_NAME_ENTERTAINER = 263,
    PHARAOH_NAME_MUSICIAN = 264,
    PHARAOH_NAME_DANCER = 265,
    PHARAOH_NAME_TEACHER = 271,
    PHARAOH_NAME_HEALTH_WORKER = 272,
    PHARAOH_NAME_EMBALMER = 273,
    PHARAOH_NAME_FIREMAN = 274,
    PHARAOH_NAME_ARCHITECT = 275,
    PHARAOH_NAME_CONSTABLE = 276,
    PHARAOH_NAME_TAX_COLLECTOR = 277,
    PHARAOH_NAME_MAGISTRATE = 278,
    PHARAOH_NAME_FERRY_BOAT = 279,
    PHARAOH_NAME_TOWER_SENTRY = 280,
    PHARAOH_NAME_ENEMY_TYPE_7 = 281,
    PHARAOH_NAME_ENEMY_TYPE_6 = 282,
    PHARAOH_NAME_ENEMY_TYPE_5 = 283,
    PHARAOH_NAME_ENEMY_TYPE_4 = 284,
    PHARAOH_NAME_ENEMY_TYPE_0 = 285,
    PHARAOH_NAME_ENEMY_TYPE_2 = 286,
    PHARAOH_NAME_ENEMY_TYPE_1 = 287,
    PHARAOH_NAME_ENEMY_TYPE_3 = 288,
    PHARAOH_NAME_ENEMY_SHIP = 289,
    PHARAOH_NAME_BEDOUIN = 290,
    PHARAOH_NAME_ENEMY_EXTRA_FIRST = 300,
    PHARAOH_NAME_ENEMY_EXTRA_LAST = 303
};

static int32_t *walker_name_counter(
    PharaohWalkerNameCounters *counters, int32_t group)
{
    if (group >= PHARAOH_NAME_ENEMY_EXTRA_FIRST)
        return &counters->by_slot[PHARAOH_NAME_ENEMY_EXTRA_LAST - group];
    return &counters->by_slot[294 - group];
}

static void walker_assign_name(PharaohWalkerQueryRecord *walker,
    PharaohWalkerNameCounters *counters, int32_t group, int32_t count)
{
    int32_t *counter = walker_name_counter(counters, group);
    walker->name_id = (int16_t)*counter;
    walker->name_group = group;
    ++*counter;
    if (*counter >= count) *counter = 0;
}

/* Complete zero-callee C translation of Pharaoh.exe 0x004B5AF0. */

void pharaoh_walker_name_set(
    PharaohWalkerQueryRecord *walkers,
    const PharaohFormationTotalsRecord formations[50],
    PharaohWalkerNameCounters *counters, int32_t walker_id)
{
    PharaohWalkerQueryRecord *walker = &walkers[walker_id];
    switch (walker->type) {
        case 6: case 14: case 63: case 65: case 72: case 95: case 97:
            return;

        case 1: case 2: case 4: case 5: case 9: case 35: case 38:
        case 54: case 55: case 56: case 75: case 79: case 80: case 81:
        case 85: case 87: case 90: case 96: case 98: case 108:
            walker_assign_name(walker, counters, PHARAOH_NAME_CITIZEN, 128);
            return;
        case 3:
            walker_assign_name(walker, counters, PHARAOH_NAME_HOMELESS, 32);
            return;
        case 7:
            walker_assign_name(walker, counters, PHARAOH_NAME_TAX_COLLECTOR, 16);
            return;
        case 8:
            walker_assign_name(walker, counters, PHARAOH_NAME_ARCHITECT, 16);
            return;
        case 10:
            walker_assign_name(walker, counters, PHARAOH_NAME_FIREMAN, 16);
            return;
        case 15: case 18: case 105:
            walker_assign_name(walker, counters, PHARAOH_NAME_ENTERTAINER, 32);
            return;
        case 16:
            walker_assign_name(walker, counters, PHARAOH_NAME_MUSICIAN, 32);
            return;
        case 17:
            walker_assign_name(walker, counters, PHARAOH_NAME_DANCER, 16);
            return;
        case 19: case 21:
            walker_assign_name(walker, counters, PHARAOH_NAME_TRADE_CARAVAN, 16);
            return;
        case 20:
            walker_assign_name(walker, counters, PHARAOH_NAME_TRADE_SHIP, 16);
            return;
        case 22: case 23:
            walker_assign_name(walker, counters, PHARAOH_NAME_LABORER, 16);
            return;
        case 26: case 39:
            walker_assign_name(walker, counters, PHARAOH_NAME_MARKET, 32);
            return;
        case 29: case 30:
            walker_assign_name(walker, counters, PHARAOH_NAME_TEACHER, 32);
            return;
        case 31: case 32: case 33:
            walker_assign_name(walker, counters, PHARAOH_NAME_HEALTH_WORKER, 32);
            return;
        case 34:
            walker_assign_name(walker, counters, PHARAOH_NAME_EMBALMER, 16);
            return;
        case 40:
            walker_assign_name(walker, counters, PHARAOH_NAME_PATRICIAN, 32);
            return;
        case 42:
            walker_assign_name(walker, counters, PHARAOH_NAME_TOWER_SENTRY, 32);
            return;
        case 43: case 44: case 45: {
            static const int16_t groups[12] = {
                PHARAOH_NAME_ENEMY_TYPE_0, PHARAOH_NAME_ENEMY_TYPE_1,
                PHARAOH_NAME_ENEMY_TYPE_2, PHARAOH_NAME_ENEMY_TYPE_3,
                PHARAOH_NAME_ENEMY_TYPE_4, PHARAOH_NAME_ENEMY_TYPE_5,
                PHARAOH_NAME_ENEMY_TYPE_6, PHARAOH_NAME_ENEMY_TYPE_7,
                300, 301, 302, 303
            };
            const int32_t enemy_type =
                formations[walker->herd_target_id_60].enemy_type;
            if (enemy_type < 0 || enemy_type >= 12) {
                walker->name_id = -1;
                walker->name_group = -1;
            } else {
                walker_assign_name(walker, counters, groups[enemy_type],
                    enemy_type == 3 ? 17 : 16);
            }
            return;
        }
        case 66:
            walker_assign_name(walker, counters, PHARAOH_NAME_DELIVERY_BOY, 16);
            return;
        case 73:
            walker_assign_name(walker, counters, PHARAOH_NAME_HUNTER, 16);
            return;
        case 76:
            walker_assign_name(walker, counters, PHARAOH_NAME_FERRY_BOAT, 16);
            return;
        case 88:
            walker_assign_name(walker, counters, PHARAOH_NAME_CONSTABLE, 16);
            return;
        case 89:
            walker_assign_name(walker, counters, PHARAOH_NAME_MAGISTRATE, 16);
            return;
        case 92: case 93: case 100: case 101:
            walker_assign_name(walker, counters, PHARAOH_NAME_ENEMY_SHIP, 16);
            return;
        case 99:
            walker_assign_name(walker, counters, PHARAOH_NAME_BEDOUIN, 16);
            return;
        default:
            walker->name_group = -2;
            return;
    }
}

void pharaoh_walker_init_scenario(
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int32_t *highest_id, int32_t *created_sequence)
{
    int32_t walker_id;
    for (walker_id = 0; walker_id < PHARAOH_WALKER_RECORD_COUNT; ++walker_id)
        pharaoh_memory_fill_bytes((uint32_t *)(void *)&walkers[walker_id],
            sizeof(walkers[walker_id]), 0);
    *highest_id = 0;
    *created_sequence = 0;
}


void pharaoh_tower_sentry_movement_ticks(
    PharaohWalkerMovementRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int32_t walker_id, int32_t ticks)
{
    PharaohWalkerMovementRecord *walker = &walkers[walker_id];
    while (ticks > 0) {
        const uint8_t next = (uint8_t)(walker->movement_tick + 1u);
        walker->movement_tick = next;
        if ((int8_t)next < 15)
            pharaoh_walker_movement_advance_tick(walkers, walker_id);
        else
            walker->movement_tick = 15;
        --ticks;
    }
}


void pharaoh_walker_movement_advance_tick(
    PharaohWalkerMovementRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int32_t walker_id)
{
    PharaohWalkerMovementRecord *walker = &walkers[walker_id];
    switch ((int8_t)walker->direction) {
        case 0:
            walker->movement_y = (int16_t)((uint16_t)walker->movement_y - 1u);
            break;
        case 1:
            walker->movement_y = (int16_t)((uint16_t)walker->movement_y - 1u);
            walker->movement_x = (int16_t)((uint16_t)walker->movement_x + 1u);
            break;
        case 2:
            walker->movement_x = (int16_t)((uint16_t)walker->movement_x + 1u);
            break;
        case 3:
            walker->movement_y = (int16_t)((uint16_t)walker->movement_y + 1u);
            walker->movement_x = (int16_t)((uint16_t)walker->movement_x + 1u);
            break;
        case 4:
            walker->movement_y = (int16_t)((uint16_t)walker->movement_y + 1u);
            break;
        case 5:
            walker->movement_y = (int16_t)((uint16_t)walker->movement_y + 1u);
            walker->movement_x = (int16_t)((uint16_t)walker->movement_x - 1u);
            break;
        case 6:
            walker->movement_x = (int16_t)((uint16_t)walker->movement_x - 1u);
            break;
        case 7:
            walker->movement_y = (int16_t)((uint16_t)walker->movement_y - 1u);
            walker->movement_x = (int16_t)((uint16_t)walker->movement_x - 1u);
            break;
        default: break;
    }
    if (walker->height_adjusted_ticks != 0u) {
        --walker->height_adjusted_ticks;
        if (walker->height_adjusted_ticks == 0u) {
            walker->ghost_visible = 0;
            return;
        }
        walker->ghost_visible = 1;
        if (walker->height_current < walker->height_target)
            ++walker->height_current;
        if (walker->height_current > walker->height_target)
            --walker->height_current;
    } else if (walker->height_current != 0u) {
        --walker->height_current;
    }
}


int32_t pharaoh_direction_is_same_or_adjacent(int32_t first, int32_t second)
{
    int32_t previous, next;
    if (first == second) return 1;
    previous = (int32_t)((uint32_t)second - 1u);
    if (previous < 0) previous = 7;
    if (first == previous) return 1;
    next = (int32_t)((uint32_t)second + 1u);
    if (next > 7) next = 0;
    return first == next;
}


void pharaoh_walker_combat_resume_activity_after_attack(
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int16_t *slot_walkers, int32_t walker_id)
{
    PharaohWalkerQueryRecord *walker = &walkers[walker_id];
    walker->combat_flag_08b = 0;
    walker->action_state = walker->action_state_before_6_7;
    walker->combat_value_090 = 0;
    walker->combat_value_08c = 0;
    walker->combat_value_08e = 0;
    pharaoh_walker_detach_slot_link(walkers, slot_walkers, walker_id);
}


int32_t pharaoh_delivery_boy_find_buyer(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id)
{
    int32_t depth;
    int32_t current = walkers[walker_id].leading_walker_id;
    for (depth = 0; depth < 8; ++depth) {
        if (walkers[current].type == 39) return current;
        current = walkers[current].leading_walker_id;
    }
    return 0;
}

_Static_assert(sizeof(PharaohWalkerMovementRecord) == PHARAOH_WALKER_RECORD_SIZE &&
    offsetof(PharaohWalkerMovementRecord, direction) == 9 &&
    offsetof(PharaohWalkerMovementRecord, movement_tick) == 0x31 &&
    offsetof(PharaohWalkerMovementRecord, movement_x) == 0x42 &&
    offsetof(PharaohWalkerMovementRecord, movement_y) == 0x44 &&
    offsetof(PharaohWalkerMovementRecord, ghost_visible) == 0x5d &&
    offsetof(PharaohWalkerMovementRecord, height_adjusted_ticks) == 0x6f &&
    offsetof(PharaohWalkerMovementRecord, height_current) == 0x70 &&
    offsetof(PharaohWalkerMovementRecord, height_target) == 0x71,
    "native walker movement overlay layout");
