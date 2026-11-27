#include "pharaoh/map/road_access.h"
#include "pharaoh/map/terrain_queries.h"

static int32_t ring_search_tile(const PharaohMapRingActorSearch *state,
    int32_t x, int32_t y, int32_t kind_count, const uint32_t *kinds,
    uint32_t flag_mask, int32_t excluded_actor)
{
    int32_t kind_index;
    const uint32_t tile = *state->search_tile;
    if (x < 0 || y < 0 || x >= state->width || y >= state->height) return 0;
    for (kind_index = 0; kind_index < kind_count; ++kind_index) {
        int32_t actor = state->tile_actor_ids[tile];
        while (actor != 0) {
            if ((uint32_t)(uint8_t)state->walkers[actor].type ==
                    kinds[kind_index] &&
                actor != excluded_actor) return actor;
            actor = state->walkers[actor].next_walker_id_on_same_tile;
        }
    }
    if ((state->tile_flags[tile] & 0x80000u) == 0u &&
        (state->tile_flags[tile] & flag_mask) != 0u) return kind_count + 1;
    return 0;
}

int32_t pharaoh_map_ring_actor_search(
    const PharaohMapRingActorSearch *state, int32_t x, int32_t y,
    int32_t kind_count, const uint32_t *kinds, uint32_t flag_mask,
    int32_t *found_x, int32_t *found_y, int32_t remaining_rings,
    int32_t excluded_actor)
{
    int32_t ring_number = 0;
    int32_t ring_length = 1;
    int32_t ring_origin = state->grid_start + y * PHARAOH_MAP_GRID_WIDTH + x;
    *found_x = x;
    *found_y = y;
    for (;;) {
        int32_t step, result;
        *state->search_tile = (uint32_t)ring_origin;
        for (step = 0; step < ring_length; ++step) {
            result = ring_search_tile(state, *found_x, *found_y, kind_count,
                kinds, flag_mask, excluded_actor);
            if (result != 0) return result;
            ++*state->search_tile;
            ++*found_x;
        }
        --*state->search_tile;
        --*found_x;
        for (step = 0; step < ring_length; ++step) {
            result = ring_search_tile(state, *found_x, *found_y, kind_count,
                kinds, flag_mask, excluded_actor);
            if (result != 0) return result;
            *state->search_tile += PHARAOH_MAP_GRID_WIDTH;
            ++*found_y;
        }
        *state->search_tile -= PHARAOH_MAP_GRID_WIDTH;
        --*found_y;
        for (step = 0; step < ring_length; ++step) {
            result = ring_search_tile(state, *found_x, *found_y, kind_count,
                kinds, flag_mask, excluded_actor);
            if (result != 0) return result;
            --*state->search_tile;
            --*found_x;
        }
        ++*state->search_tile;
        ++*found_x;
        for (step = 0; step < ring_length; ++step) {
            result = ring_search_tile(state, *found_x, *found_y, kind_count,
                kinds, flag_mask, excluded_actor);
            if (result != 0) return result;
            *state->search_tile -= PHARAOH_MAP_GRID_WIDTH;
            --*found_y;
        }
        *state->search_tile += PHARAOH_MAP_GRID_WIDTH;
        ++*found_y;
        if (remaining_rings == 0) return 0;
        ++ring_number;
        ring_length += 2;
        ring_origin -= PHARAOH_MAP_GRID_WIDTH + 1;
        --*found_x;
        --*found_y;
        --remaining_rings;
        if (ring_number > PHARAOH_MAP_GRID_WIDTH - 1) return 0;
    }
}

static int32_t pattern_has_mask(const uint32_t *terrain, int32_t map_origin,
    int32_t x, int32_t y, const int32_t pattern_offsets[][24],
    int32_t pattern, uint32_t mask)
{
    int32_t index;
    const int32_t base = map_origin + y * PHARAOH_MAP_GRID_WIDTH + x;
    for (index = 0; index < 24 && pattern_offsets[pattern][index] != 0; ++index)
        if ((terrain[base + pattern_offsets[pattern][index]] & mask) != 0) return 1;
    return 0;
}

int32_t pharaoh_map_pattern_has_water(
    const uint32_t *terrain, int32_t map_origin, int32_t x, int32_t y,
    const int32_t pattern_offsets[][24], int32_t pattern)
{
    return pattern_has_mask(terrain, map_origin, x, y, pattern_offsets, pattern, 4u);
}

_Static_assert(sizeof(PharaohWalkerQueryRecord)==388 && offsetof(PharaohWalkerQueryRecord,next_walker_id_on_same_tile)==8 && offsetof(PharaohWalkerQueryRecord,type)==10,"native ring-search walker view");

static void area_bounds(const PharaohMapTerrainView *map,
    int32_t x, int32_t y, int32_t size, int32_t radius,
    int32_t *left, int32_t *top, int32_t *right, int32_t *bottom)
{
    *left = x - radius;
    *top = y - radius;
    *right = x + size - 1 + radius;
    *bottom = y + size - 1 + radius;
    if (*left < 0) *left = 0;
    if (*top < 0) *top = 0;
    if (*right >= map->width) *right = map->width - 1;
    if (*bottom >= map->height) *bottom = map->height - 1;
}

static void map_area_apply(PharaohMapTerrainView *map,
    int32_t x, int32_t y, int32_t size, int32_t radius, uint32_t mask,
    int32_t use_and, PharaohMapAreaCursor *cursor)
{
    int32_t left, top, right, bottom, row, column;
    area_bounds(map, x, y, size, radius, &left, &top, &right, &bottom);
    cursor->offset = map->origin + top * PHARAOH_MAP_GRID_WIDTH + left;
    cursor->y = top;
    if (top > bottom) return;
    cursor->y = bottom + 1;
    for (row = top; row <= bottom; ++row) {
        cursor->x = left;
        for (column = left; column <= right; ++column) {
            if (use_and) map->terrain[cursor->offset] &= mask;
            else map->terrain[cursor->offset] |= mask;
            ++cursor->offset;
            ++cursor->x;
        }
        cursor->offset += PHARAOH_MAP_GRID_WIDTH - (right - left + 1);
    }
}

void pharaoh_map_terrain_and_area(PharaohMapTerrainView *map,
    int32_t x, int32_t y, int32_t size, int32_t radius, uint32_t mask,
    PharaohMapAreaCursor *cursor)
{
    map_area_apply(map, x, y, size, radius, mask, 1, cursor);
}

int32_t pharaoh_map_water_determine_orientation_size3(
    PharaohWaterOrientationMap *map, int32_t x, int32_t y,
    uint8_t rotate_with_view)
{
    int32_t candidate;
    if (rotate_with_view == 1 && map->view_orientation != 0) {
        if (map->view_orientation == 6) y -= 2;
        else if (map->view_orientation == 4) {
            x -= 2;
            y -= 2;
        } else if (map->view_orientation == 2) x -= 2;
    }
    if (x < 0 || y < 0 || x + 2 >= map->width || y + 2 >= map->height)
        return 999;
    const int32_t base = map->origin + y * PHARAOH_MAP_GRID_WIDTH + x;
    *map->adjusted_orientation = 0;
    for (candidate = 0; candidate < 4; ++candidate) {
        int32_t valid_cells = 0;
        int32_t marked_cells = 0;
        int32_t index;
        for (index = 0; index < 9; ++index) {
            const int32_t tile = base + map->footprint_offsets[index];
            const uint32_t flags = map->terrain[tile];
            if (map->orientation_masks[candidate][index] == 0) {
                if ((flags & 0x10004u) != 0 || map->blocking_objects[tile] != 0)
                    break;
                if ((flags & 0x40u) == 0 && (flags & 0x26341237u) == 0) {
                    if ((flags & 8u) != 0) ++marked_cells;
                } else {
                    ++marked_cells;
                }
            } else {
                if ((flags & 4u) == 0) break;
                if ((flags & 0x40u) != 0) ++marked_cells;
                if ((flags & 1u) != 0) ++marked_cells;
            }
            ++valid_cells;
        }
        if ((map->terrain[base + map->access_offsets[candidate][0]] & 4u) == 0 ||
            (map->terrain[base + map->access_offsets[candidate][1]] & 4u) == 0)
            valid_cells = 0;
        if (valid_cells == 9) {
            int32_t adjusted = candidate - map->view_orientation / 2;
            if (adjusted < 0) adjusted += 4;
            *map->raw_orientation = candidate;
            *map->adjusted_orientation = adjusted;
            return marked_cells;
        }
        *map->adjusted_orientation = candidate + 1;
    }
    return 999;
}

int32_t pharaoh_map_has_road_access_granary(
    PharaohRoadAccessContext *context, int32_t x, int32_t y)
{
    return pharaoh_map_has_road_access(context, x, y, 4);
}

int32_t pharaoh_map_general_direction(
    int32_t from_x, int32_t from_y, int32_t to_x, int32_t to_y)
{
    if (to_x < from_x) {
        if (to_y < from_y) return 7;
        if (to_y == from_y) return 6;
        return 5;
    }
    if (to_x == from_x) {
        if (to_y < from_y) return 0;
        if (to_y > from_y) return 4;
        return 8;
    }
    if (to_y < from_y) return 1;
    if (to_y == from_y) return 2;
    return 3;
}
