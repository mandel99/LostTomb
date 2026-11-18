#ifndef PHARAOH_RUNTIME_SIMULATION_TICK_H
#define PHARAOH_RUNTIME_SIMULATION_TICK_H

#include <stdint.h>
#include "pharaoh/runtime/animation_tick_counters.h"
#include "pharaoh/runtime/random_generator.h"
#include "pharaoh/runtime/city_player_countdown.h"

typedef struct PharaohSimulationTickOperations {
    void *user_data;
    void (*reduce_undo_time)(void *user_data);
    void (*dispatch_city_simulation)(void *user_data);
    void (*update_floods)(void *user_data);
    void (*update_walkers)(void *user_data);
    void (*animate_monuments)(void *user_data);
    void (*process_earthquakes)(void *user_data);
    void (*check_city_victory)(void *user_data);
} PharaohSimulationTickOperations;

typedef struct PharaohSimulationTickContext {
    const PharaohRandomGeneratorContext *random_generator;
    PharaohAnimationTickCounters *animation_counters;
    const int32_t *suppress_flood_event;
    const int32_t *flood_state;
    int32_t *osiris_flood_destroys_farms;
    int16_t *city_player_one_countdown;
    const PharaohSimulationTickOperations *operations;
} PharaohSimulationTickContext;

/* Native entry 0x004D7090. Callee operations require game-state bindings. */
void pharaoh_simulation_tick_run(const PharaohSimulationTickContext *context);

#endif
