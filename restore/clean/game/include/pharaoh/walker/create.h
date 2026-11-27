#ifndef PHARAOH_WALKER_CREATE_H
#define PHARAOH_WALKER_CREATE_H

#include <stdint.h>

#include "pharaoh/commerce/trader_records.h"
#include "pharaoh/core/crt_random.h"
#include "pharaoh/core/ring_queue.h"
#include "pharaoh/walker/record_queries.h"
#include "pharaoh/walker/tile_chain.h"

struct PharaohScenarioInvasionResetContext;

typedef struct PharaohWalkerCreateContext {
    PharaohWalkerQueryRecord *walkers;
    const PharaohFerryQueueRecord *ferry_queues;
    const PharaohGroupChildrenRecord *group_children;
    const PharaohFormationTotalsRecord *formations;
    PharaohWalkerNameCounters *name_counters;
    PharaohCrtRandom *random;
    PharaohWalkerTileChainContext *tile_chain;
    PharaohTraderRecord *trader_records;
    int32_t *next_trader_record;
    int32_t *created_sequence;
    int32_t *highest_walker_id;
    /* Native additive grid value read from DAT_00BCA2D8. */
    int32_t tile_grid_base_offset;
} PharaohWalkerCreateContext;

typedef struct PharaohWalkerReservedInitContext {
    PharaohWalkerCreateContext *walker_create;
    struct PharaohScenarioInvasionResetContext *invasion_reset;
} PharaohWalkerReservedInitContext;

/* Complete typed C translation of Pharaoh.exe 0x004B4800. */
int32_t pharaoh_walker_create(PharaohWalkerCreateContext *context,
    uint8_t owner, int32_t type, int32_t x, int32_t y, int8_t direction);

/* Complete typed C translation of Pharaoh.exe 0x004BB090. */
void pharaoh_walker_reserved_initialize(
    PharaohWalkerReservedInitContext *context);

#endif
