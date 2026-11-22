#include "pharaoh/scenario/definition_counts.h"

/* Complete C translation of Pharaoh.exe 0x00426B20. */
int32_t pharaoh_scenario_allowed_building_count(void)
{
    return 46;
}

/* Complete C translation of Pharaoh.exe 0x00426B30. */
int32_t pharaoh_monument_definition_count(void)
{
    return 38;
}

/* Complete C translation of Pharaoh.exe 0x004F4FF0. */
int32_t pharaoh_scenario_goal_text_count(const int16_t goal_text_ids[3])
{
    int32_t index, count = 0;
    for (index = 0; index < 3; ++index)
        if (goal_text_ids[index] != 0) ++count;
    return count;
}
