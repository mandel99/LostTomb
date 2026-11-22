#ifndef PHARAOH_SCENARIO_DEFINITION_COUNTS_H
#define PHARAOH_SCENARIO_DEFINITION_COUNTS_H

#include <stdint.h>

int32_t pharaoh_scenario_allowed_building_count(void);
int32_t pharaoh_monument_definition_count(void);
int32_t pharaoh_scenario_goal_text_count(const int16_t goal_text_ids[3]);

#endif
