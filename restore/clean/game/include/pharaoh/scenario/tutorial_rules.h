#ifndef PHARAOH_SCENARIO_TUTORIAL_RULES_H
#define PHARAOH_SCENARIO_TUTORIAL_RULES_H

#include <stdint.h>

typedef struct PharaohTutorialRule {
    int32_t mission_threshold;
    int32_t building_type_count;
    const int32_t *building_types;
    int32_t item_count;
    const int32_t *items;
    uint8_t reserved_14[8];
} PharaohTutorialRule;

_Static_assert(sizeof(void *)!=4 || sizeof(PharaohTutorialRule)==28,
    "native tutorial rule stride");

#endif
