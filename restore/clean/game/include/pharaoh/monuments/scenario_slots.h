#ifndef PHARAOH_MONUMENTS_SCENARIO_SLOTS_H
#define PHARAOH_MONUMENTS_SCENARIO_SLOTS_H

#include "pharaoh/building/type_queries.h"
#include "pharaoh/monuments/definitions.h"
#include "pharaoh/scenario/scenario_init_custom.h"

extern uint32_t pharaoh_pyramid_part_counts[5];
extern uint32_t pharaoh_mastaba_part_counts[3];

typedef struct PharaohMonumentSlotContext {
    const PharaohBuildingTypeRecord *buildings;
    PharaohScenarioPlayerRuntime *players;
    const int32_t *current_player;
    const int16_t *goal_definition_ids;
    const int32_t *highest_building;
    const int32_t *pyramid_style;
    const uint32_t *pyramid_parts, *mastaba_parts;
} PharaohMonumentSlotContext;

int32_t pharaoh_monument_definition_for_building(
    const PharaohMonumentSlotContext *c, int32_t building_id);
void pharaoh_monument_rebuild_player_slots(const PharaohMonumentSlotContext *c);

#endif
