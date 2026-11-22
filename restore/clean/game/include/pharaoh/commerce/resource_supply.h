#ifndef PHARAOH_COMMERCE_RESOURCE_SUPPLY_H
#define PHARAOH_COMMERCE_RESOURCE_SUPPLY_H

#include "pharaoh/commerce/empire_queries.h"
#include "pharaoh/events/event_queries.h"
#include "pharaoh/scenario/scenario_init_custom.h"

typedef struct PharaohEmpireSupplyContext {
    const PharaohEmpireCityRecord *cities;
    const PharaohEmpireTradeAmounts *route_limits;
    const int16_t *allowed_buildings;
    PharaohScenarioPlayerRuntime *players;
    const int32_t *current_player;
} PharaohEmpireSupplyContext;

typedef struct PharaohResourceAvailabilityContext {
    const PharaohEmpireSupplyContext *supply;
    const PharaohScenarioEvent *events;
    const uint32_t *event_count;
    int32_t *resource_count;
    int32_t *resources; /* 36 DWORDs */
    int32_t *food_count;
    int32_t *foods; /* 36 DWORDs */
} PharaohResourceAvailabilityContext;

/* Complete typed reuse of EconomySupply53/ResourceAvailability. Inputs use
 * native resource indices and a valid allocated scenario event range. */
uint32_t pharaoh_empire_can_produce_resource(
    const PharaohEmpireSupplyContext *c, int32_t resource);
uint32_t pharaoh_empire_can_produce_resource_potentially(
    const PharaohEmpireSupplyContext *c, int32_t resource);
uint8_t pharaoh_city_resource_available_from_events(
    const PharaohResourceAvailabilityContext *c, int32_t resource);
void pharaoh_city_resource_determine_available(
    const PharaohResourceAvailabilityContext *c);

#endif
