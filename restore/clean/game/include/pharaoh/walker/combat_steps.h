#ifndef PHARAOH_WALKER_COMBAT_STEPS_H
#define PHARAOH_WALKER_COMBAT_STEPS_H

#include "pharaoh/walker/contact.h"

typedef struct PharaohWalkerMeleeContext {
    const PharaohWalkerContactContext *contact;
    int16_t *slot_walkers;
} PharaohWalkerMeleeContext;

void pharaoh_walker_follow_chain_steps(const PharaohWalkerMotionContext *context,
    int32_t walker_id,int32_t predecessor_id,int32_t steps);
void pharaoh_walker_melee_tick(const PharaohWalkerMeleeContext *context,int32_t walker_id);

#endif
