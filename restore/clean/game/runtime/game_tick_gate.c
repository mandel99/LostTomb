#include "pharaoh/runtime/game_tick_gate.h"

static int64_t signed_dword_value(uint32_t bits)
{
    if ((bits & UINT32_C(0x80000000)) != 0) {
        return (int64_t)bits - INT64_C(0x100000000);
    }
    return bits;
}

static int32_t signed_dword_at_least(uint32_t left, uint32_t right)
{
    return signed_dword_value(left) >= signed_dword_value(right);
}

int32_t pharaoh_game_tick_gate_poll(
    PharaohGameTickGateState *state,
    const PharaohGameTickGateOperations *operations)
{
    const uint32_t current_tick = operations->time_get_time(
        operations->user_data);
    const uint32_t elapsed_bits = current_tick - state->last_ready_tick;
    uint32_t elapsed_ticks = elapsed_bits;
    const uint32_t speed_delta_bits =
        UINT32_C(100) - (uint32_t)state->game_speed;
    const int64_t signed_speed_delta = signed_dword_value(speed_delta_bits);
    int32_t speed_index = (int32_t)(signed_speed_delta / 10);

    if ((elapsed_bits & UINT32_C(0x80000000)) != 0) {
        elapsed_ticks = PHARAOH_GAME_TICK_READY_INTERVAL;
    }

    if (speed_index >= PHARAOH_GAME_TICK_SPEED_LEVEL_COUNT ||
        state->native_tick_pause_flag != 0) {
        return 0;
    }

    const uint8_t window_id = state->active_window_id;
    if (window_id == PHARAOH_GAME_TICK_SPECIAL_WINDOW_A ||
        window_id == PHARAOH_GAME_TICK_SPECIAL_WINDOW_B) {
        if (state->special_window_tick_permission != 1) {
            return 0;
        }
    } else if (state->window_tick_suppression[window_id] != 0) {
        return 0;
    }

    if (state->native_tick_blocker_a != 0 ||
        state->native_tick_blocker_b != 0) {
        return 0;
    }

    if (speed_index < 0) {
        speed_index = 0;
    }

    const uint32_t threshold_plus_padding =
        (uint32_t)state->speed_delay_ticks[speed_index] +
        PHARAOH_GAME_TICK_THRESHOLD_PADDING;
    if (signed_dword_at_least(elapsed_ticks, threshold_plus_padding) == 0) {
        return 0;
    }

    state->last_ready_tick = current_tick;
    return PHARAOH_GAME_TICK_READY_INTERVAL;
}

int32_t pharaoh_city_scroll_tick_gate_poll(uint32_t *last_ready_tick,
    int32_t scroll_speed, const PharaohGameTickGateOperations *operations)
{
    const uint32_t current_tick = operations->time_get_time(
        operations->user_data);
    const uint32_t elapsed_bits = current_tick - *last_ready_tick;
    int64_t elapsed = signed_dword_value(elapsed_bits);
    const int32_t speed_index = (int32_t)(signed_dword_value(
        UINT32_C(100) - (uint32_t)scroll_speed) / 10);
    const uint32_t threshold = (uint32_t)speed_index * UINT32_C(12) + 2u;

    if (elapsed < 0) elapsed = 10000;
    if (speed_index < 10 && elapsed >= signed_dword_value(threshold)) {
        *last_ready_tick = current_tick;
        return 1;
    }
    return 0;
}
