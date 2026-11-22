#ifndef PHARAOH_SCENARIO_SAVED_WALKER_NAMES_H
#define PHARAOH_SCENARIO_SAVED_WALKER_NAMES_H

#include "pharaoh/walker/record_queries.h"

/* Native 004D32F0: repairs all 1999 records, including inactive ones.
 * This does not allocate new names or increment the saved name counters. */
void pharaoh_saved_walker_names_repair(PharaohWalkerQueryRecord walkers[2000],
    const PharaohBuildingTypeRecord *buildings,
    const PharaohFormationTotalsRecord *formations,
    PharaohWalkerNameCounters *counters);

#endif
