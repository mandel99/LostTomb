#ifndef PHARAOH_MONUMENTS_SPHINX_WORK_H
#define PHARAOH_MONUMENTS_SPHINX_WORK_H

#include "pharaoh/rendering/monument_spot_animation.h"
#include <stdint.h>

enum { PHARAOH_SPHINX_WORK_SPOT_COUNT = 22 };

typedef struct PharaohSphinxWorkEntry {
    int32_t stage;
    int32_t walker_type;
    PharaohMonumentSpotAnimation *spot;
    int32_t reserved;
} PharaohSphinxWorkEntry;

#endif
