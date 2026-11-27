#ifndef PHARAOH_SCENARIO_TUTORIAL_RULES_H
#define PHARAOH_SCENARIO_TUTORIAL_RULES_H

#include <stdint.h>

typedef struct PharaohTutorialRule {
    int32_t mission_threshold;
    int32_t building_type_count;
    const int32_t *building_types;
    int32_t item_count;
    const int32_t *items;
    union { uint8_t reserved_14[8];
        struct { int32_t message; uint8_t reserved_18[4]; }; };
} PharaohTutorialRule;

_Static_assert(sizeof(void *)!=4 || sizeof(PharaohTutorialRule)==28,
    "native tutorial rule stride");

enum {
    PHARAOH_TUTORIAL_LAST_MISSION = 10,
    PHARAOH_TUTORIAL_MENU_FLAG_COUNT = 15,
    PHARAOH_TUTORIAL_RULE_COUNT = 26
};

/* Native entry 0x004179F0. */
void pharaoh_tutorial_mission_menu_flags(int32_t mission_id,
    int32_t advisor_access_override, const PharaohTutorialRule *rules,
    const uint8_t *tutorial_flags_a, uint8_t *tutorial_flags_b);

#endif
