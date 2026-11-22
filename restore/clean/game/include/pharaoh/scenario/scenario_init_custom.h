#ifndef PHARAOH_SCENARIO_SCENARIO_INIT_CUSTOM_H
#define PHARAOH_SCENARIO_SCENARIO_INIT_CUSTOM_H

#include <stddef.h>
#include <stdint.h>

#include "pharaoh/map/road_network_types.h"

typedef struct PharaohScenarioPlayerRuntime {
    uint8_t reserved_0000[3];
    uint8_t native_value_0003;
    uint8_t reserved_0004[8];
    int32_t treasury;
    uint8_t reserved_0010[0x0c];
    uint32_t native_value_001c;
    uint8_t reserved_0020[4];
    int32_t population;
    uint8_t reserved_0028[0x2778 - 0x28];
    uint16_t exit_x;
    uint16_t exit_y;
    int32_t exit_grid_offset;
    uint16_t quake_x;
    uint16_t quake_y;
    int32_t quake_grid_offset;
    uint8_t reserved_2788[0x298c - 0x2788];
    uint8_t food_known[9];
    uint8_t resource_known[36];
    uint8_t reserved_29b9[0x2c98 - 0x29b9];
    int32_t salary;
    int32_t rank_clamped;
    uint8_t reserved_2ca0[0x001c];
    int32_t treasury_copy;
    uint8_t reserved_2cc0[0x1618];
    int32_t starting_favor;
    uint8_t reserved_42dc[0x433a - 0x42dc];
    int16_t native_value_433a;
    uint8_t reserved_433c[0x444c - 0x433c];
    int32_t initial_rank;
    int32_t personal_savings;
    uint8_t reserved_4454[0x4460 - 0x4454];
    int16_t dock_building_ids[10];
    uint8_t reserved_4474[0x4488 - 0x4474];
    uint16_t native_value_4488;
    uint8_t reserved_448a[0x44ac - 0x448a];
    uint32_t native_value_44ac;
    uint8_t reserved_44b0[0x4630 - 0x44b0];
    uint32_t native_value_4630;
    uint8_t reserved_4634[0x46ac - 0x4634];
    PharaohRoadNetworkRank largest_road_networks[PHARAOH_ROAD_NETWORK_RANKS];
    uint8_t reserved_46fc[0x4720 - 0x46fc];
    uint32_t tutorial_active_a;
    uint32_t tutorial_active_b;
    uint8_t reserved_4728[8];
    int32_t monument_definition_ids[3];
    int32_t monument_building_ids[3];
    uint8_t reserved_4748[0x47a8 - 0x4748];
    int32_t difficulty;
    uint8_t reserved_47ac[0x4984 - 0x47ac];
    int16_t ra_trade_bonus_months;
    int16_t ra_trade_penalty_months;
    int16_t ra_trade_minor_penalty_months;
    uint8_t reserved_498a[0x49d8 - 0x498a];
} PharaohScenarioPlayerRuntime;

_Static_assert(offsetof(PharaohScenarioPlayerRuntime, native_value_0003) == 3 &&
    offsetof(PharaohScenarioPlayerRuntime, population) == 0x24 &&
    offsetof(PharaohScenarioPlayerRuntime, native_value_001c) == 0x1c &&
    offsetof(PharaohScenarioPlayerRuntime, dock_building_ids) == 0x4460 &&
    offsetof(PharaohScenarioPlayerRuntime, native_value_4488) == 0x4488 &&
    offsetof(PharaohScenarioPlayerRuntime, native_value_44ac) == 0x44ac &&
    offsetof(PharaohScenarioPlayerRuntime, native_value_4630) == 0x4630 &&
    offsetof(PharaohScenarioPlayerRuntime, largest_road_networks) == 0x46ac &&
    offsetof(PharaohScenarioPlayerRuntime, monument_definition_ids) == 0x4730 &&
    offsetof(PharaohScenarioPlayerRuntime, monument_building_ids) == 0x473c &&
    offsetof(PharaohScenarioPlayerRuntime, food_known) == 0x298c &&
    offsetof(PharaohScenarioPlayerRuntime, resource_known) == 0x2995 &&
    offsetof(PharaohScenarioPlayerRuntime, ra_trade_bonus_months) == 0x4984 &&
    offsetof(PharaohScenarioPlayerRuntime, ra_trade_penalty_months) == 0x4986 &&
    offsetof(PharaohScenarioPlayerRuntime, ra_trade_minor_penalty_months) == 0x4988 &&
    sizeof(PharaohScenarioPlayerRuntime) == 0x49d8, "native player resource fields");

typedef void (*PharaohScenarioInitStep)(void *user_data);

typedef struct PharaohScenarioInitCustomContext {
    void *user_data;
    int32_t *current_player;
    PharaohScenarioInitStep load_map;
    void (*normalize_scenario_data)(void *user_data, const void *source,
        void *destination, uint32_t byte_count);
    void *scenario_data;
    const uint32_t *editor_mode;
    int16_t *river_entry_exit;
    void (*set_river_entry)(void *user_data, int16_t x, int16_t y,
        int32_t enabled);
    PharaohScenarioInitStep post_load_steps[4];
    uint16_t *exit_coordinates;
    uint16_t *quake_coordinates;
    const uint32_t *map_width;
    const uint32_t *map_height;
    const uint32_t *map_grid_start;
    PharaohScenarioPlayerRuntime *players;
    const uint32_t *difficulty_index;
    const int32_t *difficulty_percentages;
    const int32_t *initial_funds;
    const int16_t *scenario_start_year;
    const int16_t *scenario_climate;
    const uint32_t *housing_goal_enabled;
    const int32_t *housing_goal_years;
    const uint32_t *monument_goal_enabled;
    const int32_t *monument_goal_years;
    int32_t *current_year;
    int32_t *maximum_year;
    int32_t *current_climate;
    PharaohScenarioInitStep reset_scenario_records;
    const int16_t *scroll_by_climate;
    uint32_t scroll_climate_stride;
    int32_t *scroll_x;
    int32_t *scroll_y;
    PharaohScenarioInitStep final_steps[3];
} PharaohScenarioInitCustomContext;

void pharaoh_scenario_init_custom(PharaohScenarioInitCustomContext *context);

#endif
