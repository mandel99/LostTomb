#ifndef PHARAOH_WALKER_PROJECTILE_CREATE_H
#define PHARAOH_WALKER_PROJECTILE_CREATE_H

#include "pharaoh/walker/create.h"
#include "pharaoh/walker/sound.h"
#include "pharaoh/runtime/crt_asin.h"

typedef struct PharaohProjectileCreateContext {
    PharaohWalkerCreateContext *walker_create;
    const PharaohWalkerSoundContext *sound;
    const PharaohCrtMathEnvironment *math;
    const volatile int32_t *current_player; /* native DWORD EAAC28 */
} PharaohProjectileCreateContext;

/* Native004B45C0. All contexts bind the actual shared walker and process state. */
int32_t pharaoh_walker_projectile_create(PharaohProjectileCreateContext *context,
    int32_t owner,int32_t x,int32_t y,int32_t target_x,int32_t target_y,int32_t kind);

#endif
