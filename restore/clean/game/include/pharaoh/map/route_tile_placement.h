#ifndef PHARAOH_MAP_ROUTE_TILE_PLACEMENT_H
#define PHARAOH_MAP_ROUTE_TILE_PLACEMENT_H

#include "pharaoh/map/surface_tiles.h"

typedef struct PharaohRouteTilePlacementContext {
    PharaohMapSurfaceContext *surface;
    const int32_t *world_x, *world_y; /* BCA2E8/BCA2EC, live DWORDs. */
    uint32_t *placed_count; /* BC971C, live DWORD. */
} PharaohRouteTilePlacementContext;

/* Whole native0047B310/0047B960, cdecl, logical void. These update the
 * actual terrain/property layers and the shared BC9760/64/68 scan state.
 * Every set value other than1 clears the terrain bit. The placement count
 * increments whenever the old bit is absent, including clearing calls. */
void pharaoh_map_set_road_tile(const PharaohRouteTilePlacementContext *,
    int32_t x,int32_t y,int32_t set);
void pharaoh_map_set_canal_tile(const PharaohRouteTilePlacementContext *,
    int32_t x,int32_t y,int32_t alternate,int32_t set);

#endif
