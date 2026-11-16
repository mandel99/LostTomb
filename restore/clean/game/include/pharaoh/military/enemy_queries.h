#ifndef PHARAOH_MILITARY_ENEMY_QUERIES_H
#define PHARAOH_MILITARY_ENEMY_QUERIES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct PharaohMilitaryWalker {
    uint8_t state;
    uint8_t type;
    int32_t action_state;
    int32_t wait_ticks;
    int32_t grid_offset;
} PharaohMilitaryWalker;

typedef struct PharaohFormation {
    int16_t walkers[16];
    uint8_t in_use;
    uint8_t is_legion;
    int32_t num_walkers;
    int16_t x;
    int16_t y;
    int32_t attack_type;
    int16_t destination_x;
    int16_t destination_y;
    int16_t destination_building_id;
} PharaohFormation;

typedef struct PharaohMilitaryBuilding {
    uint8_t state;
    int16_t type;
    int16_t x;
    int16_t y;
    int32_t grid_offset;
} PharaohMilitaryBuilding;

typedef void (*PharaohMilitaryMessageFunction)(
    void *user_data, int32_t message, int32_t grid_offset);
typedef void (*PharaohMilitaryRoutingFunction)(
    void *user_data, int32_t x, int32_t y);
typedef uint32_t (*PharaohMilitaryRandomFunction)(void *user_data);

typedef struct PharaohEnemyQueryContext {
    PharaohMilitaryWalker *walkers;
    size_t walker_count;
    PharaohFormation *formations;
    size_t formation_count;
    PharaohMilitaryBuilding *buildings;
    size_t building_count;
    uint8_t *soldier_strength;
    int32_t *walker_ids;
    const int32_t *routing_distance;
    int32_t grid_width;
    int32_t grid_height;
    int32_t grid_stride;
    int32_t grid_start;
    size_t grid_cell_count;
    int32_t map_result_x;
    int32_t map_result_y;
    int32_t enemies_to_leave;
    bool message_use_popup;
    const int16_t *attack_priority;
    const int16_t *attack_fallback;
    PharaohMilitaryMessageFunction post_message;
    PharaohMilitaryRoutingFunction build_enemy_routing;
    PharaohMilitaryRandomFunction random_value;
    void *user_data;
} PharaohEnemyQueryContext;

void pharaoh_set_enemies_fleeing(PharaohEnemyQueryContext *context);
void pharaoh_reset_enemy_walkers(
    PharaohEnemyQueryContext *context, int32_t formation_id);
void pharaoh_update_soldier_strength(PharaohEnemyQueryContext *context);
void pharaoh_add_soldier_strength(PharaohEnemyQueryContext *context,
    int32_t x, int32_t y, int32_t radius, int32_t amount);
int32_t pharaoh_get_maximum_soldier_strength(PharaohEnemyQueryContext *context,
    int32_t x, int32_t y, int32_t radius);
bool pharaoh_set_enemy_target_building(
    PharaohEnemyQueryContext *context, int32_t formation_id);

#endif
