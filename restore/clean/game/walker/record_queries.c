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
