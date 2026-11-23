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
