#include "pharaoh/monuments/tile_removal.h"

static void remove_singly(PharaohMonumentTileContext *c,
    const PharaohMonumentPartDesc *parts, int32_t building, bool abu_simbel)
{
    const PharaohMonumentBuilding *b = &c->buildings[building];
    const PharaohMonumentPartDesc *part = &parts[b->subtype];
    int32_t i, j;
    for (i = 0; i < part->size; ++i) for (j = 0; j < part->size; ++j) {
        int32_t x = b->x + i, y = b->y + j;
        c->tile_size_x = 1; c->tile_size_y = 1;
        c->remove_tile(c->user_data, building, x, y);
        if (abu_simbel && part->kind != c->open_ground_kind)
            c->set_terrain(c->user_data, x, y, c->monument_rock_terrain);
        else if (!abu_simbel && part->kind == c->lighthouse_tower_kind)
            c->set_terrain(c->user_data, x, y, c->rock_terrain);
    }
}

void pharaoh_abu_simbel_remove_tiles(PharaohMonumentTileContext *c,
    const PharaohMonumentPartDesc *parts, int32_t building)
{ remove_singly(c, parts, building, true); }

void pharaoh_lighthouse_remove_tiles(PharaohMonumentTileContext *c,
    const PharaohMonumentPartDesc *parts, int32_t building)
{ remove_singly(c, parts, building, false); }

void pharaoh_library_remove_tiles(PharaohMonumentTileContext *c,
    const PharaohMonumentPartDesc *parts, int32_t building)
{
    const PharaohMonumentBuilding *b = &c->buildings[building];
    int32_t size = parts[b->subtype].size, i, j;
    c->tile_size_x = size; c->tile_size_y = size;
    c->remove_tile(c->user_data, building, b->x, b->y);
    for (i = 0; i < size; ++i) for (j = 0; j < size; ++j)
        c->remove_tile(c->user_data, building, b->x + i, b->y + j);
}

void pharaoh_caesareum_remove_tiles(PharaohMonumentTileContext *c,
    const PharaohMonumentPartDesc *parts, int32_t building)
{
    const PharaohMonumentBuilding *b = &c->buildings[building];
    int32_t size = parts[b->subtype].size, i, j;
    c->tile_size_x = 1; c->tile_size_y = 1;
    for (i = 0; i < size; ++i) for (j = 0; j < size; ++j)
        c->remove_tile(c->user_data, building, b->x + i, b->y + j);
}

void pharaoh_royal_tomb_clear_tiles(PharaohMonumentTileContext *c,
    int32_t building, const PharaohMonumentPartDesc *descs, int32_t last_part)
{
    const PharaohMonumentBuilding *b = &c->buildings[building];
    int32_t part = b->subtype, size = descs[part].size, i, j;
    uint32_t terrain = part != last_part ? c->monument_rock_terrain :
        c->groundwater_terrain;
    for (i = 0; i < size; ++i) for (j = 0; j < size; ++j) {
        int32_t offset = b->x + i + (b->y + j) * c->grid_width;
        c->building_ids[offset] = 0; c->terrain[offset] = 0;
        c->bitfields[offset] &= c->destroyed_bitfield_keep;
        c->canals[offset] = 0; c->building_damage[offset] = 0;
        c->edge[offset] = (uint8_t)((c->edge[offset] & c->destroyed_edge_keep) |
            c->edge_draw_tile);
        c->sprite[offset] = 0; c->terrain[offset] = terrain;
    }
}

void pharaoh_small_royal_tomb_clear_tiles(PharaohMonumentTileContext *c,
    int32_t building)
{ pharaoh_royal_tomb_clear_tiles(c, building, c->small_tomb_descs, c->small_last_part); }
void pharaoh_medium_royal_tomb_clear_tiles(PharaohMonumentTileContext *c,
    int32_t building)
{ pharaoh_royal_tomb_clear_tiles(c, building, c->medium_tomb_descs, c->medium_last_part); }
void pharaoh_large_royal_tomb_clear_tiles(PharaohMonumentTileContext *c,
    int32_t building)
{ pharaoh_royal_tomb_clear_tiles(c, building, c->large_tomb_descs, c->large_last_part); }
void pharaoh_grand_royal_tomb_clear_tiles(PharaohMonumentTileContext *c,
    int32_t building)
{ pharaoh_royal_tomb_clear_tiles(c, building, c->grand_tomb_descs, c->grand_last_part); }
