#ifndef PHARAOH_WALKER_PROJECTILE_PATH_H
#define PHARAOH_WALKER_PROJECTILE_PATH_H

#include "pharaoh/walker/record_queries.h"

/* Native004B4180/43A0/43E0 logical WORD effects; no incidental GPR API. */
void pharaoh_walker_cross_country_update_error(volatile PharaohWalkerQueryRecord *walker);
void pharaoh_walker_cross_country_step_y(volatile PharaohWalkerQueryRecord *walker);
void pharaoh_walker_cross_country_step_x(volatile PharaohWalkerQueryRecord *walker);

typedef struct PharaohProjectilePathContext {
    PharaohWalkerQueryRecord *walkers;
    const uint32_t *terrain;
    const uint8_t *bitfields;
    const volatile int32_t *tile_grid_base_offset; /* BCA2D8 */
} PharaohProjectilePathContext;

/* Native004B4420 modifies the real scratch walker0, even on failure. */
uint32_t pharaoh_walker_projectile_path_clear(const PharaohProjectilePathContext *context,
    int32_t x,int32_t y,int32_t target_x,int32_t target_y);
/* Native004B46B0 full EAX is0/1; positive target ids retain unchecked indexing. */
uint32_t pharaoh_walker_linked_target_valid(const PharaohWalkerQueryRecord *walkers,
    int32_t walker_id);

#endif
