#ifndef PHARAOH_MAP_VENUE_TILES_H
#define PHARAOH_MAP_VENUE_TILES_H

#include <stdint.h>

typedef struct PharaohVenueTileContext {
    uint32_t *images;
    uint32_t *terrain;
    uint16_t *building_ids;
    uint8_t *building_footprints;
    const uint8_t *random;
    uint8_t *cleared;
    uint8_t *edges;
    uint32_t orientation;
    uint32_t skip_image_a;
    uint32_t plaza_image_base;
    uint32_t skip_image_b;
    uint32_t random_plaza_base;
    uint32_t default_venue_image;
    uint32_t venue_image_bases[4];
    const int32_t *venue_image_offsets[4][4];
} PharaohVenueTileContext;

struct PharaohMapSurfaceContext;
/* The caller supplies cleared (the live citizen-routing byte grid). */
void pharaoh_map_bind_venue_tiles(PharaohVenueTileContext *context,
    const struct PharaohMapSurfaceContext *surface);

void pharaoh_map_add_venue_plaza_tiles(PharaohVenueTileContext *context,
    uint32_t grid_offset, int32_t building_type, uint16_t building_id);

#endif
