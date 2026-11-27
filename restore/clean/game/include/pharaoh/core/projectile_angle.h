#ifndef PHARAOH_CORE_PROJECTILE_ANGLE_H
#define PHARAOH_CORE_PROJECTILE_ANGLE_H

#include "pharaoh/runtime/crt_asin.h"

/* Native51CAE0: wrapped signed32 deltas, inherited x87 state and logical AL.
 * Native4B45C0 stores that byte as the projectile direction. */
uint8_t pharaoh_projectile_angle(int32_t x1,int32_t y1,int32_t x2,int32_t y2,
    const PharaohCrtMathEnvironment *environment);

#endif
