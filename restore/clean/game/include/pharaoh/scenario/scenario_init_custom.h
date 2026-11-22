#ifndef PHARAOH_SCENARIO_SCENARIO_INIT_CUSTOM_H
#define PHARAOH_SCENARIO_SCENARIO_INIT_CUSTOM_H

#include <stddef.h>
#include <stdint.h>

typedef struct PharaohScenarioPlayerRuntime {
    uint8_t reserved_0000[0x000c];
    int32_t treasury;
    uint8_t reserved_0010[0x2768];
    uint16_t exit_x;
    uint16_t exit_y;
    int32_t exit_grid_offset;
    uint16_t quake_x;
    uint16_t quake_y;
    int32_t quake_grid_offset;
    uint8_t reserved_2788[0x0510];
    int32_t salary;
    int32_t rank_clamped;
    uint8_t reserved_2ca0[0x001c];
    int32_t treasury_copy;
    uint8_t reserved_2cc0[0x1618];
    int32_t starting_favor;
    uint8_t reserved_42dc[0x0170];
    int32_t initial_rank;
    int32_t personal_savings;
    uint8_t reserved_4454[0x02cc];
    uint32_t tutorial_active_a;
    uint32_t tutorial_active_b;
    uint8_t reserved_4728[0x0080];
    int32_t difficulty;
    uint8_t reserved_47ac[0x022c];
} PharaohScenarioPlayerRuntime;

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
