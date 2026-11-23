#include "pharaoh/map/venue_tiles.h"

#include "pharaoh/map/surface_tiles.h"

/* Exact native DWORD offsets at005D2078..005D23D7, four orientations per venue. */
static const int32_t venue_offsets[216] = {
    0,1,2,3,2,0,3,1,3,2,1,0,
    1,3,0,2,0,1,2,3,4,5,6,7,
    8,6,3,0,7,4,1,8,5,2,8,7,
    6,5,4,3,2,1,0,2,5,8,1,4,
    7,0,3,6,0,1,2,3,4,5,6,7,
    8,9,10,11,12,13,14,15,12,8,4,0,
    13,9,5,1,14,10,6,2,15,11,7,3,
    15,14,13,12,11,10,9,8,7,6,5,4,
    3,2,1,0,3,7,11,15,2,6,10,14,
    1,5,9,13,0,4,8,12,0,1,2,3,
    4,5,6,7,8,9,10,11,12,13,14,15,
    16,17,18,19,20,21,22,23,24,20,15,10,
    5,0,21,16,11,6,1,22,17,12,7,2,
    23,18,13,8,3,24,19,14,9,4,24,23,
    22,21,20,19,18,17,16,15,14,13,12,11,
    10,9,8,7,6,5,4,3,2,1,0,4,
    9,14,19,24,3,8,13,18,23,2,7,12,
    17,22,1,6,11,16,21,0,5,10,15,20,
};

void pharaoh_map_bind_venue_tiles(PharaohVenueTileContext *c,
    const PharaohMapSurfaceContext *s)
{
    static const uint16_t groups[4] = {381,327,319,318};
    static const uint16_t starts[4] = {0,16,52,116};
    static const uint8_t sizes[4] = {4,9,16,25};
    c->images=s->image_ids;
    c->terrain=s->map->terrain;
    c->building_ids=(uint16_t *)(void *)s->building_ids;
    c->building_footprints=s->property_flags;
    c->random=s->random;
    c->edges=s->edge_flags;
    c->orientation=(uint32_t)*s->orientation;
    c->skip_image_a=s->groups[383].first_image;
    c->plaza_image_base=s->groups[361].first_image;
    c->skip_image_b=s->groups[317].first_image;
    c->random_plaza_base=s->groups[328].first_image;
    c->default_venue_image=s->groups[437].first_image;
    for (uint32_t i=0;i<4;++i) {
        c->venue_image_bases[i]=s->groups[groups[i]].first_image;
        for (uint32_t turn=0;turn<4;++turn)
            c->venue_image_offsets[i][turn]=venue_offsets+starts[i]+turn*sizes[i];
    }
}

enum {
    PHARAOH_MAP_ROW_STRIDE = 228,
    PHARAOH_BUILDING_BANDSTAND = 31,
    PHARAOH_BUILDING_BOOTH = 30,
    PHARAOH_BUILDING_PAVILION = 33,
    PHARAOH_BUILDING_FESTIVAL_SQUARE = 209
};

/* Complete C translation of Pharaoh.exe 0x00440060. */
void pharaoh_map_add_venue_plaza_tiles(PharaohVenueTileContext *context,
    uint32_t grid_offset, int32_t building_type, uint16_t building_id)
{
    uint32_t size = 1;
    int32_t venue = -1;
    uint32_t orientation;
    uint32_t row;
    uint32_t column;

    if (building_type == PHARAOH_BUILDING_BANDSTAND) {
        size = 2;
        venue = 0;
    } else if (building_type == PHARAOH_BUILDING_BOOTH) {
        size = 3;
        venue = 1;
    } else if (building_type == PHARAOH_BUILDING_PAVILION) {
        size = 4;
        venue = 2;
    } else if (building_type == PHARAOH_BUILDING_FESTIVAL_SQUARE) {
        size = 5;
        venue = 3;
    }

    if (context->orientation == 0u) orientation = 0;
    else if (context->orientation == 2u) orientation = 1;
    else if (context->orientation == 4u) orientation = 2;
    else orientation = 3;

    for (row = 0; row < size; ++row) {
        for (column = 0; column < size; ++column) {
            const uint32_t offset = grid_offset +
                row * PHARAOH_MAP_ROW_STRIDE + column;
            const uint32_t old_image = context->images[offset];
            uint32_t old_terrain;
            uint32_t terrain;

            if (building_type != PHARAOH_BUILDING_FESTIVAL_SQUARE &&
                (old_image == context->skip_image_a ||
                 old_image == context->plaza_image_base ||
                 old_image == context->plaza_image_base + 1u ||
                 old_image == context->plaza_image_base + 2u ||
                 old_image == context->plaza_image_base + 3u ||
                 old_image == context->skip_image_b)) continue;

            old_terrain = context->terrain[offset];
            terrain = (old_terrain & 0x03052ec0u) | 8u;
            context->building_ids[offset] = building_id;
            context->terrain[offset] = terrain;
            context->building_footprints[offset] &= 0x60u;

            if ((old_terrain & 0x40u) == 0u &&
                building_type != PHARAOH_BUILDING_FESTIVAL_SQUARE) {
                context->terrain[offset] = terrain | 0x20u;
                context->images[offset] = context->random_plaza_base +
                    (context->random[offset] & 3u);
            } else {
                if (venue >= 0) {
                    context->images[offset] = context->venue_image_bases[venue] +
                        context->venue_image_offsets[venue][orientation]
                            [row * size + column];
                } else {
                    context->images[offset] = context->default_venue_image;
                }
                context->cleared[offset] = 0;
            }
            context->edges[offset] |= 0x40u;
        }
    }
}
