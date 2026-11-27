#ifndef PHARAOH_MILITARY_LEGION_REQUESTS_H
#define PHARAOH_MILITARY_LEGION_REQUESTS_H

#include "pharaoh/core/percentage_scale.h"
#include "pharaoh/military/legion_orders.h"
#include "pharaoh/walker/record_queries.h"

#include <stddef.h>
#include <stdint.h>

typedef struct PharaohTroopRequestDefinition {
    uint8_t reserved_00[12];
    int16_t required_strength;
    uint8_t reserved_0e[0x7c - 0x0e];
} PharaohTroopRequestDefinition;

typedef struct PharaohLegionRequestContext {
    PharaohNativeLegionRecord *formations;
    size_t formation_count;
    PharaohWalkerQueryRecord *walkers;
    size_t walker_count;
    uint8_t current_player;
    int32_t *current_walker_id;
    int32_t *force_request_resolution;
    const PharaohTroopRequestDefinition *request_definitions;
    int32_t request_definition_count;
} PharaohLegionRequestContext;

typedef struct PharaohTroopRequestDispatchResult {
    PharaohPercentageScaleStatus status;
    int32_t dispatched;
} PharaohTroopRequestDispatchResult;

int32_t pharaoh_walker_find_legion_transport_ship(
    PharaohLegionRequestContext *context, int32_t formation_id);
int32_t pharaoh_formation_request_strength(
    const PharaohLegionRequestContext *context, int32_t request_id,
    int32_t *warship_strength, int32_t *legion_strength);
void pharaoh_formation_request_losses(PharaohLegionRequestContext *context,
    int32_t percentage, int32_t request_id);
PharaohTroopRequestDispatchResult pharaoh_scenario_troops_request_dispatched(
    PharaohLegionRequestContext *context, int32_t request_id);

#endif
