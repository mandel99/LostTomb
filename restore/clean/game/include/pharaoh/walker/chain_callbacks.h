#ifndef PHARAOH_WALKER_CHAIN_CALLBACKS_H
#define PHARAOH_WALKER_CHAIN_CALLBACKS_H

#include "pharaoh/walker/combat_steps.h"

typedef struct PharaohWalkerChainCallbackContext {
    const PharaohWalkerMotionContext *motion;
    PharaohBuildingTypeRecord *buildings;
    const volatile int32_t *current_walker_id; /* D056F8 */
    const volatile uint32_t *orientation;      /* BCA2E0 */
    volatile int32_t *relative_direction;      /* E94E00 */
    const volatile uint8_t *rotate_spear;      /* 784CB0 */
    const volatile uint32_t *spear_sprite;     /* F55D44 */
    const volatile uint32_t *spear_rotated_sprite; /* F55CEC */
    const volatile uint32_t *spear_rotated_cart;   /* F55CF4 */
    const volatile uint32_t *carrier_sprite;   /* F55D34 */
    const uint8_t *corpse_frames;              /* 5D048C */
} PharaohWalkerChainCallbackContext;

int32_t pharaoh_walker_find_type_on_current_tile(const PharaohWalkerQueryRecord *walkers,
    const int16_t *grid_heads,int32_t walker_id,uint32_t type);
void pharaoh_walker_hunter_spear_tick(const PharaohWalkerChainCallbackContext *context);
void pharaoh_walker_carrier_tick(const PharaohWalkerChainCallbackContext *context);

#endif
