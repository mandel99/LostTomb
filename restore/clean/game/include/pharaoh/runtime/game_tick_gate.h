#ifndef PHARAOH_RUNTIME_GAME_TICK_GATE_H
#define PHARAOH_RUNTIME_GAME_TICK_GATE_H

#include <stdint.h>

enum {
    PHARAOH_GAME_TICK_WINDOW_COUNT = 256,
    PHARAOH_GAME_TICK_SPEED_LEVEL_COUNT = 10,
    PHARAOH_GAME_TICK_SPECIAL_WINDOW_A = 0x12,
    PHARAOH_GAME_TICK_SPECIAL_WINDOW_B = 0x23,
    PHARAOH_GAME_TICK_READY_INTERVAL = 10000,
    PHARAOH_GAME_TICK_THRESHOLD_PADDING = 2
};

typedef struct PharaohGameTickGateState {
    uint32_t last_ready_tick;
    int32_t game_speed;
    uint8_t native_tick_pause_flag;
    uint8_t active_window_id;
    int32_t special_window_tick_permission;
    const int32_t *window_tick_suppression;
    int32_t native_tick_blocker_a;
    int32_t native_tick_blocker_b;
    const int32_t *speed_delay_ticks;
} PharaohGameTickGateState;

typedef struct PharaohGameTickGateOperations {
    void *user_data;
    uint32_t (*time_get_time)(void *user_data);
} PharaohGameTickGateOperations;

/* Native entry 0x004DDCB0. Returns 10000 when a simulation interval is due,
   otherwise zero; updates last_ready_tick only when it returns ready. */
int32_t pharaoh_game_tick_gate_poll(
    PharaohGameTickGateState *state,
    const PharaohGameTickGateOperations *operations);

#endif
