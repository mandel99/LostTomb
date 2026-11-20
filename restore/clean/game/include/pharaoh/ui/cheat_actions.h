#ifndef PHARAOH_UI_CHEAT_ACTIONS_H
#define PHARAOH_UI_CHEAT_ACTIONS_H

#include <stdint.h>

typedef struct PharaohTreasureChestState {
    int32_t *treasury;
    int32_t *auxiliary_value;
} PharaohTreasureChestState;

typedef struct PharaohAncientAstronautOperations {
    void *user_data;
    int32_t (*monument_effect_level)(void *user_data);
    void (*apply_monument_effect)(void *user_data, int32_t amount);
} PharaohAncientAstronautOperations;

/* Complete typed translations of native cheat callbacks 0x0041CC80,
   0x0041CCF0, 0x0041CFC0 and 0x0041CFD0. */
void pharaoh_cheat_ancient_astronauts(
    const PharaohAncientAstronautOperations *operations);
void pharaoh_cheat_side_show(uint8_t *enabled);
void pharaoh_cheat_pharaohs_tomb(uint8_t *flag_a, uint8_t *flag_b);
void pharaoh_cheat_treasure_chest(const PharaohTreasureChestState *state);

#endif
