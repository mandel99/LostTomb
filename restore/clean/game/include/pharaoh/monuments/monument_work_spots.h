#ifndef PHARAOH_MONUMENTS_MONUMENT_WORK_SPOTS_H
#define PHARAOH_MONUMENTS_MONUMENT_WORK_SPOTS_H

#include "pharaoh/rendering/monument_spot_animation.h"
#include <stdint.h>

typedef struct PharaohMonumentWorkEntry {
    int32_t stage;
    int32_t walker_type;
    PharaohMonumentSpotAnimation *spot;
    int32_t reserved;
} PharaohMonumentWorkEntry;

#endif
