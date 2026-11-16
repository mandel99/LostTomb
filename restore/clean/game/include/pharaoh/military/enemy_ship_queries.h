#ifndef PHARAOH_MILITARY_ENEMY_SHIP_QUERIES_H
#define PHARAOH_MILITARY_ENEMY_SHIP_QUERIES_H
#include <stdbool.h>
#include <stdint.h>
typedef struct PharaohEnemyShipWalker {
    int32_t id, type, x, y, direction, previous_tile_direction;
    int32_t next_walker_on_tile;
} PharaohEnemyShipWalker;
typedef struct PharaohEnemyShipRules {
    int32_t direction_top_left, transport_type, warship_type, fishing_boat_type;
    int32_t grid_size, grid_start, ram_divisor_max, ram_sound_first;
    const int32_t *grid_offsets_by_direction;
} PharaohEnemyShipRules;
typedef struct PharaohEnemyShipContext {
    const PharaohEnemyShipRules *rules;
    const PharaohEnemyShipWalker *(*walker)(void *, int32_t);
    int32_t (*walker_at_grid)(void *, int32_t);
    int32_t (*water_farthest_on_line)(void *, int32_t, int32_t, int32_t, int32_t, int32_t *, int32_t *);
    void (*water_find_free_tile)(void *, int32_t *, int32_t *);
    void (*play_sound)(void *, int32_t);
    void *user_data;
} PharaohEnemyShipContext;
int32_t pharaoh_enemy_ship_facing_direction(const PharaohEnemyShipWalker *, const PharaohEnemyShipRules *);
void pharaoh_enemy_ship_pick_escape_point(PharaohEnemyShipContext *, int32_t *, int32_t *, const int32_t[3], const int32_t[3], const PharaohEnemyShipWalker *);
void pharaoh_enemy_ship_play_ram_sound(PharaohEnemyShipContext *, int32_t);
int32_t pharaoh_enemy_ship_find_ship_at(PharaohEnemyShipContext *, int32_t, int32_t);
int32_t pharaoh_enemy_ship_find_ram_target(PharaohEnemyShipContext *, const PharaohEnemyShipWalker *);
void pharaoh_enemy_ship_reroute_tick(int32_t *reroute_ticks, int32_t last_tick,
    void (*remove_route)(void *, int32_t), void *user_data, int32_t walker_id);
#endif
