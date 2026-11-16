#ifndef PHARAOH_MILITARY_TRANSPORT_BOATS_H
#define PHARAOH_MILITARY_TRANSPORT_BOATS_H

#include <stddef.h>
#include <stdint.h>

typedef struct PharaohTransportWalker {
    int32_t type, action_state, action_before_attack, direction, wait_ticks;
    int32_t ship_formation_id;
    int16_t x, y, previous_x, previous_y, landing_x, landing_y;
    int32_t grid_offset, cross_x, cross_y;
    uint8_t state, alternative_location_index, troops_on_board;
} PharaohTransportWalker;

typedef struct PharaohTransportFormation {
    int16_t *walkers;
    int32_t num_walkers;
    int16_t order_x, order_y, x, y;
    int32_t ship_walker_id;
    uint8_t is_at_fort;
} PharaohTransportFormation;

typedef struct PharaohTransportRules {
    int32_t formation_max_walkers;
    int32_t action_corpse, action_on_ship, action_sailing;
    int32_t action_going_to_legion, action_evading;
    int32_t direction_destination, direction_reroute, direction_lost;
    int32_t walker_state_dead, drown_tick, face_enemy_wait;
} PharaohTransportRules;

typedef struct PharaohTransportContext {
    PharaohTransportWalker *walkers;
    size_t walker_count;
    PharaohTransportFormation *formations;
    size_t formation_count;
    const PharaohTransportRules *rules;
    int32_t (*walker_speed)(void *, int32_t);
    int32_t (*grid_offset)(void *, int32_t, int32_t);
    void (*move_ticks)(void *, int32_t, int32_t);
    void (*route_remove)(void *, int32_t);
    void (*map_walker_delete)(void *, int32_t);
    void (*map_walker_add)(void *, int32_t);
    void (*handle_corpse)(void *, int32_t);
    void (*calculate_formations)(void *);
    void (*arrived_at_dock)(void *, int32_t, int32_t *);
    void (*water_landing_point)(void *, int32_t, int32_t, int32_t, int32_t,
        int32_t *, int32_t *);
    void (*nearest_free_land)(void *, int32_t *, int32_t *);
    void (*legion_move_to)(void *, int32_t, int32_t, int32_t, int32_t);
    void (*face_enemy)(void *, int32_t);
    void *user_data;
} PharaohTransportContext;

int32_t pharaoh_transport_legion_ready(PharaohTransportContext *, int32_t);
void pharaoh_transport_legion_board(PharaohTransportContext *, int32_t);
void pharaoh_transport_legion_land(PharaohTransportContext *, int32_t,
    int32_t, int32_t);
void pharaoh_transport_sink(PharaohTransportContext *, int32_t);
void pharaoh_transport_sail_to_dock(PharaohTransportContext *, int32_t,
    int32_t *);
void pharaoh_transport_sail_to_legion(PharaohTransportContext *, int32_t,
    int32_t *);
void pharaoh_transport_sail_to_landing(PharaohTransportContext *, int32_t,
    int32_t *);
void pharaoh_transport_docked(PharaohTransportContext *, int32_t);
void pharaoh_transport_enemy_move(PharaohTransportContext *, int32_t, int32_t *);

#endif
