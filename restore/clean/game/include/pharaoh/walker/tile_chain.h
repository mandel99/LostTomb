#ifndef PHARAOH_WALKER_TILE_CHAIN_H
#define PHARAOH_WALKER_TILE_CHAIN_H

#include "pharaoh/walker/record_queries.h"

typedef struct PharaohWalkerTileChainContext {
    PharaohWalkerQueryRecord *walkers;
    int16_t *grid_heads;
} PharaohWalkerTileChainContext;

void pharaoh_walker_attach_to_tile(PharaohWalkerTileChainContext *context,
    int32_t walker_id); /* 004B52F0 */
void pharaoh_walker_detach_from_tile(PharaohWalkerTileChainContext *context,
    int32_t walker_id); /* 004B5420 */
void pharaoh_walker_refresh_tile_ordinal(PharaohWalkerTileChainContext *context,
    int32_t walker_id); /* 004B53B0, zero callees */

#endif
