#ifndef PHARAOH_MAP_MOISTURE_H
#define PHARAOH_MAP_MOISTURE_H
#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/core/x87_numeric.h"
uint8_t pharaoh_map_moisture_flood_objects_around(
    PharaohFloodTile *const *objects, int32_t tile);
uint8_t pharaoh_map_opposite_pair_has_mask(
    const uint32_t *terrain, int32_t offset, uint32_t mask);
uint32_t pharaoh_map_moisture_or_wet(const uint32_t *terrain,
    PharaohFloodTile *const *objects, const uint8_t *moisture, int32_t tile);
uint32_t pharaoh_map_moisture_neighbour_shade(const uint32_t *terrain,
    PharaohFloodTile *const *objects, const uint8_t *moisture,
    int32_t tile, int32_t offset);
int32_t pharaoh_map_nearest_terrain_ring(PharaohMapSurfaceContext *c,int32_t x,int32_t y,uint32_t mask,int32_t radius_limit);
int32_t pharaoh_map_fertility_at_distance(int32_t distance,int32_t limit);
uint32_t pharaoh_map_adjust_moisture_neighbors(PharaohMapSurfaceContext *c,uint32_t tile);
void pharaoh_map_rebuild_fertility(PharaohMapSurfaceContext *c,int32_t x0,int32_t y0,int32_t x1,int32_t y1);
uint8_t pharaoh_map_extend_floodplain(PharaohMapSurfaceContext *c,int32_t x,int32_t y);
uint8_t pharaoh_map_remove_blocked_floodplain(PharaohMapSurfaceContext *c,int32_t x,int32_t y);
uint8_t pharaoh_map_trim_floodplain_spurs(PharaohMapSurfaceContext *c,int32_t x,int32_t y);
void pharaoh_map_refresh_painted_region(PharaohMapSurfaceContext *c,int32_t x,int32_t y,int32_t radius);
void pharaoh_map_normalize_floodplain_region(PharaohMapSurfaceContext *c,int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_map_normalize_floodplain_all(PharaohMapSurfaceContext *c);
void pharaoh_map_revalidate_water_images(PharaohMapSurfaceContext *c);
void pharaoh_map_set_dune_image(PharaohMapSurfaceContext *c,int32_t x,int32_t y,int32_t set);
void pharaoh_map_refresh_dune_region(PharaohMapSurfaceContext *c,int32_t x0,int32_t y0,int32_t x1,int32_t y1);
#endif
