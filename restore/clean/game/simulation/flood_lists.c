#include "pharaoh/simulation/flood_build.h"
#include "pharaoh/runtime/platform_measure_speed.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

_Static_assert(sizeof(void *) != 4 ||
    (sizeof(PharaohFloodList) == 28 && sizeof(PharaohFloodTile) == 28 &&
     sizeof(PharaohFloodSortedLink) == 16 &&
     offsetof(PharaohFloodWorld, lists) == 0x32c64 &&
     offsetof(PharaohFloodWorld, progress) == 0x32fb8 &&
     offsetof(PharaohFloodWorld, has_floodplain) == 0x32fbc),
    "native flood object layout changed");

static void *allocate(PharaohFloodListsContext *c, uint32_t bytes)
{
    return c->memory != NULL ?
        c->memory->allocate(c->memory->user_data, bytes) : malloc(bytes);
}

static void release(PharaohFloodListsContext *c, void *p)
{
    if (c->memory != NULL) c->memory->release(c->memory->user_data, p);
    else free(p);
}

void pharaoh_flood_list_clear(PharaohFloodListsContext *c, PharaohFloodList *list)
{
    if (list->count == 0) return;
    PharaohFloodSortedLink *scan = list->head_link;
    list->cursor_tile = scan != NULL ? scan->item : NULL;
    if (scan != NULL) scan = scan->next;
    while (list->cursor_tile != NULL) {
        PharaohFloodTile *const next = scan != NULL ? scan->item : NULL;
        if (scan != NULL) scan = scan->next;
        PharaohFloodTile *const tile = list->cursor_tile;
        PharaohFloodSortedLink *const link = tile->sorted_link;
        PharaohFloodSortedLink *const previous = link->previous;
        PharaohFloodSortedLink *const following = link->next;
        if (previous != NULL) previous->next = following;
        else list->head_link = following;
        if (following != NULL) following->previous = previous;
        else list->tail_link = previous;
        link->previous = NULL;
        link->next = NULL;
        release(c, link);
        tile->sorted_link = NULL;
        PharaohFloodTile *const current = list->cursor_tile;
        if (current != NULL) {
            pharaoh_actor_walking_no_operation();
            release(c, current);
        }
        list->cursor_tile = next;
    }
    list->count = 0;
}

PharaohFloodTile *pharaoh_flood_list_insert(PharaohFloodListsContext *c,
    PharaohFloodList *list, int32_t tile_id, int32_t x, int32_t y,
    int32_t depth, uint8_t water)
{
    PharaohFloodTile *tile = allocate(c, sizeof(*tile));
    if (tile != NULL)
        tile = pharaoh_flood_tile_initialize(tile, tile_id, x, y, depth, list, water);
    const int32_t key = c->random[tile_id];
    PharaohFloodSortedLink *link = allocate(c, sizeof(*link));
    if (link != NULL) {
        link->previous = NULL;
        link->next = NULL;
        link->item = tile;
        link->key = key;
    }
    /* Possible original bug: the native NULL-allocation paths still reach
     * these dereferences. The verified domain uses successful allocations. */
    if (list->head_link == NULL || list->head_link->key >= key) {
        link->next = list->head_link;
        if (list->head_link != NULL) list->head_link->previous = link;
        link->previous = NULL;
        list->head_link = link;
        if (list->tail_link == NULL) list->tail_link = link;
    } else if (list->tail_link->key <= key) {
        link->previous = list->tail_link;
        if (list->tail_link != NULL) list->tail_link->next = link;
        link->next = NULL;
        list->tail_link = link;
        if (list->head_link == NULL) list->head_link = link;
    } else {
        PharaohFloodSortedLink *at = list->head_link;
        while (at->key < key) at = at->next;
        link->next = at;
        link->previous = at->previous;
        at->previous->next = link;
        at->previous = link;
    }
    tile->sorted_link = link;
    list->count = (int32_t)((uint32_t)list->count + 1u);
    return tile;
}

void pharaoh_floods_reset(PharaohFloodListsContext *c)
{
    PharaohFloodWorld *const world = c->world;
    memset(world->tiles, 0, sizeof(world->tiles));
    for (int32_t i = 0; i < 30; ++i) {
        PharaohFloodList *const list = &world->lists[i];
        pharaoh_flood_list_clear(c, list);
        list->recede_passes = 0;
        list->depth = i + 1;
        list->owner = world;
        list->cursor_tile = NULL;
        list->count = 0;
    }
    world->current_list = 30;
    world->season_month = 0;
    world->has_floodplain = 0;
}

static void reset(void *user_data)
{ pharaoh_floods_reset(user_data); }
static PharaohFloodTile *insert(void *user_data, PharaohFloodList *list,
    int32_t tile, int32_t x, int32_t y, int32_t depth, uint8_t water)
{ return pharaoh_flood_list_insert(user_data, list, tile, x, y, depth, water); }

void pharaoh_flood_build_bind_lists(PharaohFloodBuildContext *build,
    PharaohFloodBuildOperations *ops, PharaohFloodListsContext *c)
{
    build->lists = c->world->lists;
    build->tiles = c->world->tiles;
    build->progress = &c->world->progress;
    build->has_floodplain = &c->world->has_floodplain;
    build->operations = ops;
    ops->user_data = c;
    ops->reset = reset;
    ops->insert = insert;
}
