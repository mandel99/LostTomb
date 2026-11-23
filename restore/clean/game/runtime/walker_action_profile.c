#include "pharaoh/runtime/walker_action_profile.h"
#include <string.h>

void pharaoh_walker_action_profile_initialize(
    PharaohWalkerActionProfileState *state,
    PharaohProfileQueryCounter query_frequency)
{
    /* Native QPF writes persistent storage; a failing endpoint may leave
       it untouched. Only the low DWORD is consumed by the timer code. */
    int64_t frequency = state->high_resolution_frequency;
    state->high_resolution_available =
        query_frequency(state->clock_user_data, &frequency) != 0;
    state->high_resolution_frequency = (uint32_t)frequency;
    memset(state->entries, 0, PHARAOH_WALKER_ACTION_PROFILE_COUNT *
        sizeof(*state->entries));
}
