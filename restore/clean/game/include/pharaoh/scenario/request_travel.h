#ifndef PHARAOH_SCENARIO_REQUEST_TRAVEL_H
#define PHARAOH_SCENARIO_REQUEST_TRAVEL_H
#include "pharaoh/commerce/empire_queries.h"
#include "pharaoh/events/event_queries.h"

typedef struct PharaohRequestTravelContext {
    const PharaohEmpireObjectRecord *objects; /*77F748,200x98*/
    const PharaohEmpireCityRecord *cities; /*77DE00,61x106*/
    PharaohEmpireRouteRecord *routes; /*776FB0,50x324*/
} PharaohRequestTravelContext;

/* Complete0044C110. Native ECX event pointer becomes a typed argument.
   The route search's flag mutations remain observable even when it fails. */
int32_t pharaoh_request_travel_months(const PharaohRequestTravelContext *,
    const PharaohScenarioEvent *event);
#endif
