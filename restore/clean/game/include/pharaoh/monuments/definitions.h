#ifndef PHARAOH_MONUMENTS_DEFINITIONS_H
#define PHARAOH_MONUMENTS_DEFINITIONS_H

#include <stdint.h>

typedef struct PharaohMonumentDefinition {
    int16_t kind;
    int16_t subkind;
    int16_t size;
    int16_t resources[3];
    uint8_t unknown_0c[4];
} PharaohMonumentDefinition;

_Static_assert(sizeof(PharaohMonumentDefinition) == 0x10,
    "native monument-definition size changed");

extern PharaohMonumentDefinition pharaoh_monument_definitions[38];

uint8_t pharaoh_monument_definition_uses_resource(
    const PharaohMonumentDefinition *definitions,
    int32_t definition, int32_t resource);
int32_t pharaoh_scenario_monument_slot_obelisk_type(
    const PharaohMonumentDefinition *definitions,
    const int16_t *goal_definition_ids, int32_t slot);
uint8_t pharaoh_scenario_monument_slot_is_pyramid(
    const PharaohMonumentDefinition *definitions,
    const int16_t *goal_definition_ids, int32_t slot);
int32_t pharaoh_scenario_get_pyramid_style(
    const PharaohMonumentDefinition *definitions,
    const int16_t goal_definition_ids[3]);
int32_t pharaoh_scenario_get_extra_script_monument(
    const PharaohMonumentDefinition *definitions,
    const int16_t goal_definition_ids[2]);
int32_t pharaoh_scenario_get_mausoleum_size(
    const PharaohMonumentDefinition *definitions,
    const int16_t goal_definition_ids[2]);

#endif
