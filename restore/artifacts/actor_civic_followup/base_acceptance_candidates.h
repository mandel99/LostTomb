#ifndef RESTORE_BASE_ACCEPTANCE_CANDIDATES_H
#define RESTORE_BASE_ACCEPTANCE_CANDIDATES_H
#include <stdint.h>

/* Logical AL results. Native physical results remain with the RAW owners. */
uint8_t restore_actor_accept_base_kind227(uint32_t receiver, int32_t building,
    int32_t actor, uint32_t accept);
uint8_t restore_actor_accept_base_kind228(uint32_t receiver, int32_t building,
    int32_t actor, uint32_t accept);
uint8_t restore_actor_accept_base_kind225(uint32_t receiver, int32_t building,
    int32_t actor, uint32_t accept);
uint8_t restore_actor_accept_kind230_mode0(uint32_t receiver,
    int32_t building, int32_t actor, uint32_t accept);
#endif
