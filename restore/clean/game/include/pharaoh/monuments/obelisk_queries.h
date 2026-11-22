#ifndef PHARAOH_MONUMENTS_OBELISK_QUERIES_H
#define PHARAOH_MONUMENTS_OBELISK_QUERIES_H

#include "pharaoh/building/type_queries.h"
#include "pharaoh/scenario/scenario_init_custom.h"

/* Native004E59C0 returns full EAX; the old void decompilation loses it. */
int32_t pharaoh_obelisk_first_unfinished(
    const PharaohBuildingTypeRecord *buildings,
    const PharaohScenarioPlayerRuntime *players, const int32_t *current_player);

#endif
