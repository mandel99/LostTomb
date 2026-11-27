#ifndef PHARAOH_WALKER_CONTACT_H
#define PHARAOH_WALKER_CONTACT_H

#include "pharaoh/walker/sound.h"
#include "pharaoh/walker/tile_chain.h"
#include "pharaoh/core/crt_random.h"
#include "pharaoh/building/type_queries.h"
#include "pharaoh/military/formation_counts.h"
#include "pharaoh/military/legion_orders.h"

/* Native EA5C88 + player*49D8. Unknown semantics retain address labels. */
typedef struct PharaohWalkerContactPlayerRecord {
    uint32_t counter_ea5c88;
    uint8_t reserved_004[0x60-4];
    int16_t native_ea5ce8;
    uint8_t reserved_062[0x314-0x62];
    int32_t building_ea5f9c;
    uint8_t reserved_318[0x49d8-0x318];
} PharaohWalkerContactPlayerRecord;

typedef struct PharaohWalkerContactContext {
    PharaohWalkerQueryRecord *walkers;
    const PharaohWalkerTypeProperties *properties;
    PharaohNativeLegionRecord *formations;
    const PharaohFormationCountContext *formation_counts;
    const PharaohBuildingTypeRecord *buildings;
    PharaohWalkerContactPlayerRecord *players;
    const volatile int32_t *current_player;
    PharaohCrtRandom *random;
    const PharaohWalkerSoundContext *sound;
    const int16_t *grid_heads;
    const int32_t *adjacent_grid_offsets; /* eight DWORDs5F80FC */
} PharaohWalkerContactContext;

void pharaoh_walker_kind_sound(const PharaohWalkerContactContext *context,int32_t walker_id);
void pharaoh_formation_contact_loss(const PharaohWalkerContactContext *context,int32_t formation_id);
void pharaoh_walker_contact_damage(const PharaohWalkerContactContext *context,int32_t walker_id,int32_t target_id);
void pharaoh_walker_contact_chain(const PharaohWalkerContactContext *context,int32_t walker_id,int32_t candidate_id);
void pharaoh_walker_scan_neighbor_contacts(const PharaohWalkerContactContext *context,int32_t walker_id);

typedef struct PharaohWalkerMotionContext {
    PharaohWalkerTileChainContext *tile_chain;
    const uint32_t *terrain;
    const uint8_t *bridge_sprites;
    const volatile int32_t *tile_grid_base_offset;
    const PharaohWalkerContactContext *contact;
} PharaohWalkerMotionContext;

int32_t pharaoh_walker_cross_country_advance(const PharaohWalkerMotionContext *context,
    int32_t walker_id,int32_t steps);
void pharaoh_walker_move_to_next_tile(const PharaohWalkerMotionContext *context,int32_t walker_id);

#endif
