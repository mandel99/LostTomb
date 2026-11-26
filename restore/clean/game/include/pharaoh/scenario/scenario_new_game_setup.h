#ifndef PHARAOH_SCENARIO_SCENARIO_NEW_GAME_SETUP_H
#define PHARAOH_SCENARIO_SCENARIO_NEW_GAME_SETUP_H

#include <stdint.h>
#include "pharaoh/events/event_queries.h"

typedef void (*PharaohScenarioSetupStep)(void *user_data);

typedef struct PharaohScenarioPlayerInitContext {
    uint8_t *player_records;
    uint32_t *word_00ea186c;
    uint32_t *flag_00ea1820;
} PharaohScenarioPlayerInitContext;

void pharaoh_scenario_player_custom_init(
    PharaohScenarioPlayerInitContext *context);

typedef struct PharaohScenarioInvasionResetContext {
    /* Native bindings: events at 0x00785D74; count at 0x00785D70. */
    PharaohScenarioEvent *events;
    const int32_t *event_count;
    /* Native globals: 0x0078A634 and 0x005D9A40. */
    uint8_t *invasion_search_flag;
    int32_t *current_event_index;
    /* Five contiguous native DWORDs beginning at 0x00D0562C. */
    uint32_t *invasion_amounts;
} PharaohScenarioInvasionResetContext;

void pharaoh_scenario_invasion_amounts_clear(
    PharaohScenarioInvasionResetContext *context);

typedef struct PharaohScenarioRuntimeStateResetContext {
    uint32_t (*words_78d7a0)[17];
    uint32_t (*words_78d5c0)[12];
    uint32_t *word_78d78c;
    uint32_t (*words_78d3ec)[51];
    uint32_t (*words_78d4b8)[51];
} PharaohScenarioRuntimeStateResetContext;

void pharaoh_scenario_runtime_state_reset(
    const PharaohScenarioRuntimeStateResetContext *state);

typedef struct PharaohScenarioNewGameSetupContext {
    void *user_data;
    uint8_t *victory_state;
    uint8_t *victory_forced;
    uint8_t *victory_cheated;
    uint32_t *editor_construction_tool;
    PharaohScenarioPlayerInitContext *player_init;
    int32_t *current_player;
    uint8_t *player_flag_2;
    uint8_t *player_flag_3;
    PharaohScenarioSetupStep reset_steps_before_invasion[12];
    PharaohScenarioInvasionResetContext *invasion_reset;
    PharaohScenarioSetupStep reset_steps_after_invasion[3];
    PharaohScenarioRuntimeStateResetContext *runtime_reset;
    const uint32_t *editor_mode;
    int32_t *current_climate;
    PharaohScenarioSetupStep editor_steps[5];
    PharaohScenarioSetupStep initialize_custom_scenario;
    const char *current_player_name;
    char *player_name;
    uint8_t *player_name_initialized;
    const uint32_t *difficulty_index;
    const int32_t *difficulty_percentages;
    int32_t *player_treasury;
} PharaohScenarioNewGameSetupContext;


void pharaoh_scenario_new_game_setup(
    PharaohScenarioNewGameSetupContext *context);

#endif
