#include "pharaoh/monuments/obelisk_queries.h"

int32_t pharaoh_obelisk_first_unfinished(
    const PharaohBuildingTypeRecord *buildings,
    const PharaohScenarioPlayerRuntime *players, const int32_t *current_player)
{
    const int32_t player = *current_player;
    int32_t result = 0;
    for (int32_t slot = 0; slot < 3; ++slot) {
        if (result != 0) return result;
        const int32_t id = players[player].monument_building_ids[slot];
        if (id != 0 && buildings[id].state == 1 &&
            (buildings[id].building_type == 207 ||
             buildings[id].building_type == 208) &&
            buildings[id].pyramid_work_phase != 100)
            result = id;
    }
    return result;
}
