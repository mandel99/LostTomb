#ifndef PHARAOH_RUNTIME_WALKER_ACTION_PROFILE_H
#define PHARAOH_RUNTIME_WALKER_ACTION_PROFILE_H

#include <stdint.h>

typedef struct PharaohWalkerActionProfileEntry {
    uint32_t calls;
    uint32_t total_ticks;
    uint32_t maximum_ticks;
    uint32_t reserved_0c;
    double total_seconds;
    double maximum_seconds;
} PharaohWalkerActionProfileEntry;

typedef int32_t (*PharaohProfileQueryCounter)(
    void *user_data, int64_t *counter);
typedef uint32_t (*PharaohProfileGetTickCount)(void *user_data);

enum { PHARAOH_WALKER_ACTION_PROFILE_COUNT = 110 };

typedef struct PharaohWalkerActionProfileState {
    PharaohWalkerActionProfileEntry *entries;
    uint32_t active_index;
    uint32_t start_tick;
    uint32_t end_tick;
    uint8_t high_resolution_available;
    uint32_t high_resolution_frequency;
    uint32_t start_counter_low;
    int64_t end_counter;
    PharaohProfileQueryCounter query_counter;
    PharaohProfileGetTickCount get_tick_count;
    void *clock_user_data;
} PharaohWalkerActionProfileState;

void pharaoh_walker_action_profile_begin(
    PharaohWalkerActionProfileState *state, uint32_t profile_index);
/* Native entry 0x00489A80. */
void pharaoh_walker_action_profile_end(
    PharaohWalkerActionProfileState *state);
void pharaoh_walker_action_profile_initialize(
    PharaohWalkerActionProfileState *state,
    PharaohProfileQueryCounter query_frequency);

#endif
