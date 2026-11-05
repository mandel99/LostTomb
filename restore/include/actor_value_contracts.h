#ifndef RESTORE_ACTOR_VALUE_CONTRACTS_H
#define RESTORE_ACTOR_VALUE_CONTRACTS_H
#include "monument_worker_motion.h"
enum RestorePyramidSearchActorField {
 RESTORE_PYRAMID_SEARCH_ROUTE_RESET=0x44,
 RESTORE_PYRAMID_SEARCH_CLAIM_ROOT=0x5e
};

uint8_t restore_obelisk_advance_progress(int32_t building);
int32_t restore_pyramid_select_idle_piece_or_access(int32_t actor,int32_t mode);
uint32_t restore_pyramid_assign_available_worker(int32_t actor);

/* Integration endpoints: bind to the retained physical providers, not the
   generated src declarations whose DAT pointer units are incorrect.
   4E5840 consumes three cdecl DWORD arguments and publishes memory effects.
   4F3000 consumes actor/piece and returns its computed target ID in EAX;
   its native body overwrites EAX before reading it and has no receiver. */
void restore_obelisk_refresh_progress(int32_t building,int32_t x,int32_t y);
uint32_t restore_monument_worker_publish_target_eax(int32_t actor,int32_t piece);
#endif
