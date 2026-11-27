#include "pharaoh/building/entertainment_construction.h"

#include "pharaoh/building/construction_commit_support.h"
#include "pharaoh/core/memory_strings.h"
#include "pharaoh/map/terrain_queries.h"

_Static_assert(sizeof(PharaohBuildingTypeRecord) == 264 &&
    offsetof(PharaohBuildingTypeRecord, runtime_value_038) == 0x38 &&
    offsetof(PharaohBuildingTypeRecord, next_part_building_id) == 0x3a &&
    offsetof(PharaohBuildingTypeRecord, venue_grid_offset) == 0xd0 &&
    offsetof(PharaohBuildingTypeRecord, venue_orientation) == 0xd4 &&
    offsetof(PharaohBuildingTypeRecord, venue_has_plaza) == 0xd8 &&
    offsetof(PharaohFloodTile, is_floodplain) == 0x18,
    "native entertainment building and transient tile fields");

const int32_t pharaoh_entertainment_bandstand_ranks[9] = {
    0, 1, 0, 1, 2, 1, 0, 1, 0
};
const int32_t pharaoh_entertainment_pavilion_ranks[16] = {
    0, 1, 1, 0, 1, 2, 2, 1, 1, 2, 2, 1, 0, 1, 1, 0
};
const uint8_t pharaoh_entertainment_pattern_2[4] = {1, 1, 1, 1};
const uint8_t pharaoh_entertainment_pattern_3[9] = {
    0, 1, 0, 1, 0, 1, 0, 1, 0
};
const uint8_t pharaoh_entertainment_pattern_4[16] = {
    0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0
};
const uint8_t pharaoh_entertainment_pattern_5[25] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static int32_t add32(int32_t a, uint32_t b)
{
    return (int32_t)((uint32_t)a + b);
}

static uint32_t venue_extent(PharaohEntertainmentKind kind)
{
    return kind == PHARAOH_ENTERTAINMENT_BOOTH ? 2u :
        kind == PHARAOH_ENTERTAINMENT_BANDSTAND ? 3u :
        kind == PHARAOH_ENTERTAINMENT_PAVILION ? 4u : 5u;
}

uint32_t pharaoh_entertainment_site_valid(
    const PharaohEntertainmentConstructionContext *c,
    PharaohEntertainmentKind kind)
{
    PharaohMapSurfaceContext *const s = c->allocation->surface;
    const uint32_t oy = *c->selection_y, ox = *c->selection_x;
    const uint32_t size = venue_extent(kind);
    uint32_t row, column, map_row;
    int terrain_clear = 1, unoccupied = 1, access = 0;

    if ((int32_t)oy >= (int32_t)(oy + size)) return 0;
    map_row = ox + oy * 228u;
    for (row = 0; row < size; ++row) {
        for (column = 0; (int32_t)ox < (int32_t)(ox + size) &&
            column < size; ++column) {
            uint32_t tile, terrain;
            const PharaohFloodTile *object;
            if (pharaoh_construction_venue_pattern((int32_t)column,
                    (int32_t)row, (int32_t)size, c->patterns_by_size) != 0 &&
                pharaoh_construction_venue_is_crossing(s->map->terrain,
                    s->map->origin, (int32_t)(ox + column),
                    (int32_t)(oy + row), (int32_t)ox, (int32_t)oy,
                    (int32_t)size)) access = 1;
            /* Origin is reloaded after both genuine child algorithms. */
            tile = (uint32_t)s->map->origin + map_row + column;
            object = s->flood_tiles[tile];
            terrain = s->map->terrain[tile];
            if (object != NULL && object->is_floodplain == 0)
                terrain_clear = 0;
            if ((terrain & 0xeeffd77fu) != 0 && (terrain & 0x40u) == 0)
                terrain_clear = 0;
            if (s->building_ids[tile] > 0) unoccupied = 0;
        }
        map_row += 228u;
    }
    return access && terrain_clear && unoccupied ? 1u : 0u;
}

uint32_t pharaoh_entertainment_select_slot(
    const PharaohEntertainmentConstructionContext *c,
    int32_t x, int32_t y, int32_t *out_x, int32_t *out_y,
    PharaohEntertainmentKind primary, PharaohEntertainmentKind secondary)
{
    const uint32_t extent = venue_extent(primary);
    uint32_t rows = 1, columns = 1, rotated = 0, count, index, chosen = 0;
    uint32_t dx, dy, cell;
    int32_t score = 5, locations[50];

    /* Constructor secondary kinds initialize both native fallback words. */
    locations[0] = 0;
    locations[1] = 0;
    if (secondary == PHARAOH_ENTERTAINMENT_BANDSTAND) {
        columns = 2; locations[0] = 1; locations[1] = 1;
    } else if (secondary == PHARAOH_ENTERTAINMENT_PAVILION) {
        rows = 2; columns = 2; locations[0] = 2; locations[1] = 2;
    }
    (void)pharaoh_construction_venue_free_spots(c->occupancy,
        (int32_t)extent, (int32_t)rows, (int32_t)columns, &count, locations);
    if ((int32_t)count <= 0) {
        if (secondary == PHARAOH_ENTERTAINMENT_BANDSTAND) {
            rotated = 1;
            (void)pharaoh_construction_venue_free_spots(c->occupancy,
                (int32_t)extent, (int32_t)columns, (int32_t)rows,
                &count, locations);
        }
        if ((int32_t)count <= 0) {
            *out_x = add32(x, (uint32_t)locations[0]);
            *out_y = add32(y, (uint32_t)locations[1]);
            return UINT32_MAX;
        }
    }
    for (index = 0; index < count; ++index) {
        int32_t rank;
        if (extent == 2u) break;
        dx = (uint32_t)locations[index * 2u];
        dy = (uint32_t)locations[index * 2u + 1u];
        rank = extent == 3u ? c->bandstand_ranks[dx * 3u + dy] :
            extent == 4u ? c->pavilion_ranks[dx * 4u + dy] : 0;
        if (rank < score) { score = rank; chosen = index; }
    }
    dx = (uint32_t)locations[chosen * 2u];
    dy = (uint32_t)locations[chosen * 2u + 1u];
    *out_x = add32(x, dx);
    *out_y = add32(y, dy);
    cell = dx * 5u + dy;
    c->occupancy[cell] = 1;
    if (secondary == PHARAOH_ENTERTAINMENT_BANDSTAND)
        c->occupancy[cell + (rotated != 0 ? 5u : 1u)] = 1;
    else if (secondary == PHARAOH_ENTERTAINMENT_PAVILION) {
        c->occupancy[cell + 5u] = 1;
        c->occupancy[cell + 1u] = 1;
        c->occupancy[cell + 6u] = 1;
    }
    return rotated;
}

static void fill_road_grid(const PharaohEntertainmentConstructionContext *c,
    int32_t x, int32_t y, PharaohEntertainmentKind kind)
{
    const PharaohMapTerrainView *const map = c->allocation->surface->map;
    pharaoh_construction_venue_fill_road_grid(c->occupancy, map->terrain,
        map->origin, x, y, (int32_t)kind);
}

static void restore_preview(const PharaohEntertainmentConstructionContext *c)
{
    PharaohMapSurfaceContext *const s = c->allocation->surface;
    pharaoh_memory_copy_forward_words64(c->backup->saved_terrain,
        s->map->terrain, 0x32c40);
    pharaoh_memory_copy_forward_words64(c->backup->saved_bytes[0],
        c->venue_tiles->cleared, 0xcb10);
    pharaoh_memory_copy_forward_words64(c->backup->saved_bytes[1],
        s->property_flags, 0xcb10);
    pharaoh_memory_copy_forward_words64(c->backup->saved_bytes[2],
        s->edge_flags, 0xcb10);
    pharaoh_map_restore_free_tile_images(s, c->backup->saved_images);
}

static void publish_plaza(const PharaohEntertainmentConstructionContext *c,
    uint32_t tile, PharaohEntertainmentKind kind, uint16_t id)
{
    pharaoh_map_bind_venue_tiles(c->venue_tiles, c->allocation->surface);
    pharaoh_map_add_venue_plaza_tiles(c->venue_tiles, tile, (int32_t)kind, id);
}

uint32_t pharaoh_entertainment_build_booth(
    const PharaohEntertainmentConstructionContext *c,
    uint32_t preview, int32_t x, int32_t y)
{
    PharaohMapSurfaceContext *const s = c->allocation->surface;
    uint32_t width = (uint32_t)s->map->width, height, tile, id, image, owner;
    int32_t bx, by;

    if (x >= (int32_t)width) x = (int32_t)(width - 1u);
    height = (uint32_t)s->map->height;
    if (y >= (int32_t)height) y = (int32_t)(height - 1u);
    tile = (uint32_t)s->map->origin + (uint32_t)y * 228u + (uint32_t)x;
    if (pharaoh_entertainment_site_valid(c, PHARAOH_ENTERTAINMENT_BOOTH) == 0)
        return 0;
    fill_road_grid(c, x, y, PHARAOH_ENTERTAINMENT_BOOTH);
    if (pharaoh_entertainment_select_slot(c, x, y, &bx, &by,
            PHARAOH_ENTERTAINMENT_BOOTH, PHARAOH_ENTERTAINMENT_BOOTH) ==
        UINT32_MAX) return 0;
    if (preview != 0) return 1;
    restore_preview(c);
    owner = *c->allocation->current_player;
    *c->placed_count = 1;
    id = (uint32_t)pharaoh_building_allocate(c->allocation, owner, 31,
        bx, by, 0, 0);
    image = s->groups[383].first_image;
    s->buildings[id].venue_has_plaza = 1;
    s->buildings[id].venue_grid_offset = tile;
    pharaoh_map_place_surface_square(s, (uint16_t)id, bx, by, 1, 1, image, 8);
    (void)pharaoh_map_building_undo_capture(s->undo, (int32_t)id);
    publish_plaza(c, tile, PHARAOH_ENTERTAINMENT_BOOTH, (uint16_t)id);
    return 1;
}

uint32_t pharaoh_entertainment_build_bandstand(
    const PharaohEntertainmentConstructionContext *c,
    uint32_t preview, int32_t x, int32_t y)
{
    PharaohMapSurfaceContext *const s = c->allocation->surface;
    const uint32_t tile = (uint32_t)s->map->origin +
        (uint32_t)y * 228u + (uint32_t)x;
    uint32_t rotation, first, second, booth, image;
    int32_t bx, by, cx, cy, first_image, second_image, sx, sy;

    fill_road_grid(c, x, y, PHARAOH_ENTERTAINMENT_BANDSTAND);
    if (pharaoh_entertainment_site_valid(c, PHARAOH_ENTERTAINMENT_BANDSTAND) == 0)
        return 0;
    rotation = pharaoh_entertainment_select_slot(c, x, y, &bx, &by,
        PHARAOH_ENTERTAINMENT_BANDSTAND, PHARAOH_ENTERTAINMENT_BANDSTAND);
    if (rotation == UINT32_MAX) return 0;
    if (pharaoh_entertainment_select_slot(c, x, y, &cx, &cy,
            PHARAOH_ENTERTAINMENT_BANDSTAND, PHARAOH_ENTERTAINMENT_BOOTH) ==
        UINT32_MAX) return 0;
    if (preview != 0) return 1;
    restore_preview(c);
    *c->placed_count = 1;
    /* Native valid callers use even view orientations 0..6 and rotation0/1. */
    pharaoh_venue_orientation_images(*s->orientation, &s->groups[361].first_image,
        (int32_t)rotation, &first_image, &second_image);
    first = (uint32_t)pharaoh_building_allocate(c->allocation,
        *c->allocation->current_player, 30, bx, by, 0, 0);
    s->buildings[first].venue_has_plaza = 1;
    s->buildings[first].venue_orientation = (int32_t)rotation;
    s->buildings[first].venue_grid_offset = tile;
    pharaoh_map_place_surface_square(s, (uint16_t)first, bx, by,
        1, 1, (uint32_t)first_image, 8);
    (void)pharaoh_map_building_undo_capture(s->undo, (int32_t)first);
    sx = add32(bx, rotation != 0 ? 1u : 0u);
    sy = add32(by, rotation != 0 ? 0u : 1u);
    second = (uint32_t)pharaoh_building_allocate(c->allocation,
        *c->allocation->current_player, 30, sx, sy, 0, 0);
    pharaoh_map_place_surface_square(s, (uint16_t)second, sx, sy,
        1, 1, (uint32_t)second_image, 8);
    s->buildings[second].venue_grid_offset = tile;
    s->buildings[first].venue_orientation = (int32_t)rotation;
    s->buildings[second].venue_has_plaza = 0;
    (void)pharaoh_map_building_undo_capture(s->undo, (int32_t)second);
    booth = (uint32_t)pharaoh_building_allocate(c->allocation,
        *c->allocation->current_player, 31, cx, cy, 0, 0);
    image = s->groups[383].first_image;
    s->buildings[booth].venue_has_plaza = 0;
    s->buildings[booth].venue_grid_offset = tile;
    pharaoh_map_place_surface_square(s, (uint16_t)booth, cx, cy, 1, 1, image, 8);
    (void)pharaoh_map_building_undo_capture(s->undo, (int32_t)booth);
    s->buildings[first].next_part_building_id = (int16_t)second;
    s->buildings[second].runtime_value_038 = (int16_t)first;
    s->buildings[second].next_part_building_id = (int16_t)booth;
    s->buildings[booth].runtime_value_038 = (int16_t)second;
    publish_plaza(c, tile, PHARAOH_ENTERTAINMENT_BANDSTAND, (uint16_t)first);
    return 1;
}

uint32_t pharaoh_entertainment_build_pavilion(
    const PharaohEntertainmentConstructionContext *c,
    uint32_t preview, int32_t x, int32_t y)
{
    PharaohMapSurfaceContext *const s = c->allocation->surface;
    uint32_t width = (uint32_t)s->map->width, height, tile, rotation;
    uint32_t pavilion, booth, first, second, image, owner;
    int32_t px, py, bx, by, cx, cy, first_image, second_image, sx, sy;

    if (x >= (int32_t)width) x = (int32_t)(width - 1u);
    height = (uint32_t)s->map->height;
    if (y >= (int32_t)height) y = (int32_t)(height - 1u);
    tile = (uint32_t)s->map->origin + (uint32_t)y * 228u + (uint32_t)x;
    fill_road_grid(c, x, y, PHARAOH_ENTERTAINMENT_PAVILION);
    if (pharaoh_entertainment_site_valid(c, PHARAOH_ENTERTAINMENT_PAVILION) == 0)
        return 0;
    if (pharaoh_entertainment_select_slot(c, x, y, &px, &py,
            PHARAOH_ENTERTAINMENT_PAVILION, PHARAOH_ENTERTAINMENT_PAVILION) ==
        UINT32_MAX) return 0;
    rotation = pharaoh_entertainment_select_slot(c, x, y, &bx, &by,
        PHARAOH_ENTERTAINMENT_PAVILION, PHARAOH_ENTERTAINMENT_BANDSTAND);
    if (rotation == UINT32_MAX) return 0;
    if (pharaoh_entertainment_select_slot(c, x, y, &cx, &cy,
            PHARAOH_ENTERTAINMENT_PAVILION, PHARAOH_ENTERTAINMENT_BOOTH) ==
        UINT32_MAX) return 0;
    if (preview != 0) return 1;
    restore_preview(c);
    owner = *c->allocation->current_player;
    *c->placed_count = 1;
    pavilion = (uint32_t)pharaoh_building_allocate(c->allocation, owner, 33,
        px, py, 0, 0);
    image = s->groups[317].first_image;
    s->buildings[pavilion].venue_has_plaza = 1;
    s->buildings[pavilion].venue_grid_offset = tile;
    pharaoh_map_place_surface_square(s, (uint16_t)pavilion, px, py, 2, 2, image, 8);
    (void)pharaoh_map_building_undo_capture(s->undo, (int32_t)pavilion);
    booth = (uint32_t)pharaoh_building_allocate(c->allocation,
        *c->allocation->current_player, 31, cx, cy, 0, 0);
    image = s->groups[383].first_image;
    s->buildings[booth].venue_has_plaza = 0;
    s->buildings[booth].venue_grid_offset = tile;
    pharaoh_map_place_surface_square(s, (uint16_t)booth, cx, cy, 1, 1, image, 8);
    (void)pharaoh_map_building_undo_capture(s->undo, (int32_t)booth);
    pharaoh_venue_orientation_images(*s->orientation, &s->groups[361].first_image,
        (int32_t)rotation, &first_image, &second_image);
    first = (uint32_t)pharaoh_building_allocate(c->allocation,
        *c->allocation->current_player, 30, bx, by, 0, 0);
    s->buildings[first].venue_has_plaza = 0;
    s->buildings[first].venue_orientation = (int32_t)rotation;
    s->buildings[first].venue_grid_offset = tile;
    pharaoh_map_place_surface_square(s, (uint16_t)first, bx, by,
        1, 1, (uint32_t)first_image, 8);
    (void)pharaoh_map_building_undo_capture(s->undo, (int32_t)first);
    sx = add32(bx, rotation != 0 ? 1u : 0u);
    sy = add32(by, rotation != 0 ? 0u : 1u);
    second = (uint32_t)pharaoh_building_allocate(c->allocation,
        *c->allocation->current_player, 30, sx, sy, 0, 0);
    pharaoh_map_place_surface_square(s, (uint16_t)second, sx, sy,
        1, 1, (uint32_t)second_image, 8);
    s->buildings[second].venue_has_plaza = 0;
    s->buildings[second].venue_grid_offset = tile;
    s->buildings[first].venue_orientation = (int32_t)rotation;
    s->buildings[booth].venue_orientation = (int32_t)rotation;
    s->buildings[pavilion].venue_orientation = (int32_t)rotation;
    (void)pharaoh_map_building_undo_capture(s->undo, (int32_t)second);
    s->buildings[pavilion].next_part_building_id = (int16_t)booth;
    s->buildings[booth].runtime_value_038 = (int16_t)pavilion;
    s->buildings[booth].next_part_building_id = (int16_t)first;
    s->buildings[first].runtime_value_038 = (int16_t)booth;
    s->buildings[first].next_part_building_id = (int16_t)second;
    s->buildings[second].runtime_value_038 = (int16_t)first;
    publish_plaza(c, tile, PHARAOH_ENTERTAINMENT_PAVILION, (uint16_t)pavilion);
    return 1;
}

uint32_t pharaoh_entertainment_build_festival_square(
    const PharaohEntertainmentConstructionContext *c,
    uint32_t preview, int32_t x, int32_t y)
{
    PharaohMapSurfaceContext *const s = c->allocation->surface;
    PharaohEntertainmentPlayer *city = &c->players[*c->allocation->current_player];
    uint32_t width, height, tile, id;

    if (city->festival_square_x != UINT32_MAX &&
        city->festival_square_y != UINT32_MAX) return 0;
    if (pharaoh_entertainment_site_valid(c,
            PHARAOH_ENTERTAINMENT_FESTIVAL_SQUARE) == 0) return 0;
    if (preview != 0) return 1;
    restore_preview(c);
    width = (uint32_t)s->map->width;
    if (x >= (int32_t)width) x = (int32_t)(width - 1u);
    height = (uint32_t)s->map->height;
    if (y >= (int32_t)height) y = (int32_t)(height - 1u);
    tile = (uint32_t)s->map->origin + (uint32_t)y * 228u;
    *c->placed_count = 1;
    city = &c->players[*c->allocation->current_player];
    city->festival_square_x = (uint32_t)x;
    tile += (uint32_t)x;
    city->festival_square_y = (uint32_t)y;
    fill_road_grid(c, x, y, PHARAOH_ENTERTAINMENT_FESTIVAL_SQUARE);
    id = (uint32_t)pharaoh_building_allocate(c->allocation,
        *c->allocation->current_player, 209, x, y, 0, 0);
    (void)pharaoh_map_building_undo_capture(s->undo, (int32_t)id);
    s->buildings[id].venue_grid_offset = tile;
    s->buildings[id].venue_has_plaza = 1;
    publish_plaza(c, tile, PHARAOH_ENTERTAINMENT_FESTIVAL_SQUARE, (uint16_t)id);
    c->players[*c->allocation->current_player].festival_square_placed = 1;
    return 1;
}
