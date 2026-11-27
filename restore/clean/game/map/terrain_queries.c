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

void pharaoh_map_visit_diamond_rings(
    const PharaohMapGridGeometry *geometry, int32_t center_tile,
    int32_t radius, PharaohMapRingVisitor visitor, void *context)
{
    int32_t raw_top = center_tile / PHARAOH_MAP_GRID_WIDTH;
    int32_t raw_bottom = raw_top;
    const int32_t diagonal = center_tile % PHARAOH_MAP_GRID_WIDTH - raw_top;
    int32_t ring;

    for (ring = 0; ring < radius; ++ring, --raw_top, ++raw_bottom) {
        int32_t left = diagonal + raw_top;
        int32_t right = diagonal + raw_bottom;
        int32_t top = raw_top;
        int32_t bottom = raw_bottom;
        const int32_t left_inside = left >= geometry->diamond_x_min;
        const int32_t right_inside = right <= geometry->diamond_x_max;
        const int32_t top_inside = top >= geometry->diamond_y_min;
        const int32_t bottom_inside = bottom <= geometry->diamond_y_max;
        int32_t coordinate;

        if (!left_inside) left = geometry->diamond_x_min;
        if (!right_inside) right = geometry->diamond_x_max;
        if (!top_inside) top = geometry->diamond_y_min;
        if (!bottom_inside) bottom = geometry->diamond_y_max;
        if (top_inside || bottom_inside) {
            for (coordinate = left; coordinate <= right; ++coordinate) {
                if (top_inside &&
                    (uint8_t)visitor((uint32_t)(coordinate +
                        top * PHARAOH_MAP_GRID_WIDTH),
                        coordinate, top, context) == 0u) return;
                if (bottom_inside && top < bottom &&
                    (uint8_t)visitor((uint32_t)(coordinate +
                        bottom * PHARAOH_MAP_GRID_WIDTH),
                        coordinate, bottom, context) == 0u) return;
            }
        }
        if (left_inside || right_inside) {
            for (coordinate = top + 1; coordinate <= bottom - 1; ++coordinate) {
                if (left_inside &&
                    (uint8_t)visitor((uint32_t)(left +
                        coordinate * PHARAOH_MAP_GRID_WIDTH),
                        left, coordinate, context) == 0u) return;
                if (right_inside && left < right &&
                    (uint8_t)visitor((uint32_t)(right +
                        coordinate * PHARAOH_MAP_GRID_WIDTH),
                        right, coordinate, context) == 0u) return;
            }
        }
    }
}

int32_t pharaoh_map_can_place_road_under_canal(
    const uint32_t *terrain, const int32_t *image_ids,
    int32_t canal_group_first_image, int32_t view_orientation,
    int32_t tile)
{
    int32_t image_offset = image_ids[tile] - canal_group_first_image;
    int32_t east_west;

    if (image_offset > 47) image_offset -= 48;
    switch (image_offset) {
        case 0: case 7: case 9: case 15: case 17:
        case 19: case 21: case 28: case 30: case 36:
        case 38: case 40: case 42: case 44: case 46:
            east_west = 1;
            break;
        case 1: case 6: case 8: case 16: case 18:
        case 20: case 22: case 27: case 29: case 37:
        case 39: case 41: case 43: case 45: case 47:
            east_west = 0;
            break;
        default:
            return 0;
    }
    if (view_orientation == 2 || view_orientation == 6)
        east_west ^= 1;
    if (east_west != 0) {
        if ((terrain[tile + 1] & 0x40u) != 0) return 0;
        if ((terrain[tile - 1] & 0x40u) != 0) return 0;
    } else {
        if ((terrain[tile - PHARAOH_MAP_GRID_WIDTH] & 0x40u) != 0) return 0;
        if ((terrain[tile + PHARAOH_MAP_GRID_WIDTH] & 0x40u) != 0) return 0;
    }
    return 1;
}

uint8_t pharaoh_map_tile_is_buildable_for_editor_at(
    const uint32_t *terrain, const int8_t *height_values, int32_t offset)
{
    if ((terrain[offset] & 0x10000u) != 0) return 0;
    if (height_values[offset] > 4) return 1;
    if (height_values[offset] < 1) return 0;
    return (uint8_t)((terrain[offset] >> 13) & 1u);
}

int32_t pharaoh_map_diagonal_pair_has_mask(
    const uint32_t *terrain, int32_t offset, uint32_t mask)
{
    return ((terrain[offset - PHARAOH_MAP_GRID_WIDTH] & mask) != 0 ||
            (terrain[offset + PHARAOH_MAP_GRID_WIDTH] & mask) != 0) &&
        ((terrain[offset + 1] & mask) != 0 ||
         (terrain[offset - 1] & mask) != 0);
}

int32_t pharaoh_map_pattern_has_mask_4000(
    const uint32_t *terrain, int32_t map_origin, int32_t x, int32_t y,
    const int32_t pattern_offsets[][24], int32_t pattern)
{
    return pattern_has_mask(terrain, map_origin, x, y, pattern_offsets, pattern, 0x4000u);
}

uint8_t pharaoh_construction_area_has_entry_point(
    const PharaohConstructionEntryPoints *points,
    int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    const int32_t left = x1 < x2 ? x1 : x2;
    const int32_t right = x1 > x2 ? x1 : x2;
    const int32_t top = y1 < y2 ? y1 : y2;
    const int32_t bottom = y1 > y2 ? y1 : y2;
    int32_t index;
    for (index = 0; index < 4; ++index)
        if (points->primary_x[index] >= left &&
            points->primary_x[index] <= right &&
            points->primary_y[index] >= top &&
            points->primary_y[index] <= bottom) return 1;
    for (index = 0; index < 4; ++index)
        if (points->secondary_x[index] >= left &&
            points->secondary_x[index] <= right &&
            points->secondary_y[index] >= top &&
            points->secondary_y[index] <= bottom) return 1;
    return 0;
}

int32_t pharaoh_map_region_matches_predicate(
    const PharaohMapTerrainView *map, int32_t x, int32_t y,
    int32_t size, int32_t radius, PharaohMapTerrainPredicate predicate,
    void *user_data, int32_t *result_x, int32_t *result_y)
{
    int32_t left = x - radius;
    int32_t right = x + size - 1 + radius;
    int32_t bottom = y + size - 1 + radius;
    uint32_t tile;

    *result_y = y - radius;
    if (left < 0) left = 0;
    if (*result_y < 0) *result_y = 0;
    if (right >= map->width) right = map->width - 1;
    if (bottom >= map->height) bottom = map->height - 1;
    tile = (uint32_t)map->origin +
        (uint32_t)*result_y * PHARAOH_MAP_GRID_WIDTH + (uint32_t)left;
    for (;;) {
        if (*result_y > bottom) return 0;
        *result_x = left;
        if (left <= right) {
            do {
                if ((uint8_t)predicate(map->terrain[tile], user_data) != 0u)
                    return 1;
                ++tile;
                ++*result_x;
            } while (*result_x <= right);
        }
        tile += (uint32_t)(left - right + PHARAOH_MAP_GRID_WIDTH - 1);
        ++*result_y;
    }
}

uint8_t pharaoh_construction_venue_pattern(
    int32_t x, int32_t y, int32_t size,
    const uint8_t *const patterns_by_size[4])
{
    if (size < 2 || size > 5) return 1;
    return patterns_by_size[size - 2][x * size + y];
}
uint32_t pharaoh_map_region_terrain_count_reached(
    const PharaohMapTerrainView *map,int32_t x,int32_t y,int32_t size,
    int32_t radius,uint32_t bits,int32_t required,
    int32_t *result_x,int32_t *result_y)
{
    int32_t left=(int32_t)((uint32_t)x-(uint32_t)radius);
    int32_t right=(int32_t)((uint32_t)x+(uint32_t)size+(uint32_t)radius-1u);
    int32_t top=(int32_t)((uint32_t)y-(uint32_t)radius);
    int32_t bottom=(int32_t)((uint32_t)y+(uint32_t)size+(uint32_t)radius-1u);
    if(left<0)left=0;
    if(top<0)top=0;
    if(right>=map->width)right=(int32_t)((uint32_t)map->width-1u);
    if(bottom>=map->height)bottom=(int32_t)((uint32_t)map->height-1u);
    *result_y=top;
    uint32_t index=(uint32_t)map->origin+(uint32_t)top*228u+(uint32_t)left;
    const uint32_t stride=(uint32_t)left-(uint32_t)right+227u;
    uint32_t remaining=(uint32_t)required;
    if(top<=bottom){
        uint32_t rows=(uint32_t)bottom-(uint32_t)top+1u;
        *result_y=(int32_t)(rows+(uint32_t)top);
        do {
            *result_x=left;
            if(left<=right){
                uint32_t columns=(uint32_t)right-(uint32_t)left+1u;
                uint32_t tile=index;index+=columns;
                *result_x=(int32_t)(columns+(uint32_t)left);
                do {if((map->terrain[tile]&bits)!=0u)--remaining;++tile;--columns;}while(columns!=0u);
            }
            index+=stride;--rows;
        }while(rows!=0u);
    }
    return remaining==0u || (remaining&UINT32_C(0x80000000))!=0u;
}

int32_t pharaoh_map_ferry_landing_at(
    const PharaohFerryLandingMap *map, int32_t x, int32_t y)
{
    int32_t tile;
    int32_t building_id;

    if (map->view_orientation == 6) --y;
    else if (map->view_orientation == 4) { --x; --y; }
    else if (map->view_orientation == 2) --x;

    if (x < 0 || y < 0 || x + 1 >= map->width || y + 1 >= map->height)
        return 999;
    tile = map->origin + y * PHARAOH_MAP_GRID_WIDTH + x;
    building_id = map->building_ids[tile];
    if (building_id == 0 || map->buildings[building_id].building_type != 136)
        return 0;
    if (map->building_ids[tile + 1] != building_id ||
        map->building_ids[tile + PHARAOH_MAP_GRID_WIDTH] != building_id ||
        map->building_ids[tile + PHARAOH_MAP_GRID_WIDTH + 1] != building_id)
        return 0;
    return 1;
}

int32_t pharaoh_map_is_ferry_landing_at_offset(
    const PharaohFerryLandingMap *map, int32_t grid_offset)
{
    const int32_t relative = grid_offset - map->origin;
    return pharaoh_map_ferry_landing_at(
        map, relative % PHARAOH_MAP_GRID_WIDTH,
        relative / PHARAOH_MAP_GRID_WIDTH);
}
