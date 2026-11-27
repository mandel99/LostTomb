#ifndef PHARAOH_MAP_BRIDGE_PLACEMENT_H
#define PHARAOH_MAP_BRIDGE_PLACEMENT_H

#include "pharaoh/scenario/scene_routing.h"
#include <stddef.h>
#include <stdint.h>

/* Actual contiguous native DWORDs BC96F4..BC9703. Volatile preserves the
 * native intermediate direction store and early-rejection length store. */
typedef struct PharaohBridgePlacementState {
    volatile uint32_t direction;
    volatile int32_t length;
    volatile int32_t available;
    volatile uint32_t tile_step;
} PharaohBridgePlacementState;

_Static_assert(sizeof(PharaohBridgePlacementState) == 16 &&
    offsetof(PharaohBridgePlacementState, length) == 4 &&
    offsetof(PharaohBridgePlacementState, available) == 8 &&
    offsetof(PharaohBridgePlacementState, tile_step) == 12 &&
    sizeof(PharaohSavedGameGeometry) == 16 &&
    offsetof(PharaohSavedGameGeometry, grid_start) == 8 &&
    offsetof(PharaohSavedGameGeometry, grid_border) == 12,
    "native bridge state and routing geometry DWORD layout");

typedef struct PharaohBridgePlacementContext {
    PharaohBridgePlacementState *state;
    PharaohSceneRouting *routing;
    uint8_t *tile_direction; /* Mutable BYTE grid ADC36C. */
} PharaohBridgePlacementContext;

/* Whole native0047F530, cdecl(x,y,mode), logical void. Mode0 selects a low
 * bridge; every nonzero mode uses ship-bridge pieces, but only mode==1 adds
 * its length>=5 guard and chooses a special middle segment.
 *
 * routing->terrain is the actual AF598C DWORD grid; routing->tile_direction
 * aliases tile_direction. Its live geometry is BCA2D0(width,height,origin,
 * border); routing->map_orientation is actual BCA2E0. Full bordered arrays,
 * building records and real ferry providers are required by the existing
 * citizen routing child when it encounters active ferries. No callback or
 * routing result is substituted by this owner. */
void pharaoh_map_place_bridge(const PharaohBridgePlacementContext *context,
    int32_t x,int32_t y,int32_t mode);

#endif
