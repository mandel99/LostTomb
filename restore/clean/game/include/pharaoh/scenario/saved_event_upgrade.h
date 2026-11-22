#ifndef PHARAOH_SCENARIO_SAVED_EVENT_UPGRADE_H
#define PHARAOH_SCENARIO_SAVED_EVENT_UPGRADE_H

#include "pharaoh/events/event_queries.h"

/* Native 004D40C0. The five default prices are resources 31..35. */
void pharaoh_saved_events_upgrade_resources(const int16_t *count,
    PharaohScenarioEvent *events, int32_t prices[5][2],
    const int32_t default_prices[5][2]);

#endif
