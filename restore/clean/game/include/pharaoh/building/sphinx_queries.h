#ifndef PHARAOH_BUILDING_SPHINX_QUERIES_H
#define PHARAOH_BUILDING_SPHINX_QUERIES_H

#include <stdint.h>

enum { PHARAOH_SPHINX_WORK_SPOT_COUNT = 22 };
enum { PHARAOH_WALKER_CARPENTER = 79 };

typedef struct PharaohSphinxWorkSpot {
    int32_t stage;
    int32_t walker_type;
    uint8_t reserved_08[8];
} PharaohSphinxWorkSpot;

void pharaoh_sphinx_get_work_percentages(
    const PharaohSphinxWorkSpot spots[PHARAOH_SPHINX_WORK_SPOT_COUNT],
    int16_t current_stage,
    int32_t *carpenter_percent, int32_t *mason_percent,
    int32_t *carpenters, int32_t *carpenters_done,
    int32_t *masons, int32_t *masons_done);

#endif
