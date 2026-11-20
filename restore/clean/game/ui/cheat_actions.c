#include "pharaoh/ui/cheat_actions.h"

void pharaoh_cheat_ancient_astronauts(
    const PharaohAncientAstronautOperations *operations)
{
    if (operations->monument_effect_level(operations->user_data) != 0)
        operations->apply_monument_effect(operations->user_data, 2);
}

void pharaoh_cheat_side_show(uint8_t *enabled)
{
    *enabled = *enabled == 0;
}

void pharaoh_cheat_pharaohs_tomb(uint8_t *flag_a, uint8_t *flag_b)
{
    *flag_a = 1;
    *flag_b = 1;
}

void pharaoh_cheat_treasure_chest(const PharaohTreasureChestState *state)
{
    const int32_t treasury = *state->treasury;
    if (treasury < 15000) {
        *state->treasury = (int32_t)((uint32_t)treasury + 1000u);
        *state->auxiliary_value =
            (int32_t)((uint32_t)*state->auxiliary_value + 1000u);
    }
}
