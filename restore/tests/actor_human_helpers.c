#include "actor_callbacks_human.h"
#define FUN_004b1d50 restore_human_timer_body
#define FUN_004b3250 restore_human_route_body
#define restore_actor_human_boundary restore_human_boundary_body
#ifdef RESTORE_ACTOR_HUMAN_PRODUCTION
#include "../src/unit_01416.c"
#include "../src/unit_01427.c"
#include "../runtime/actor_callbacks_human_boundary.c"
#else
#include "../artifacts/actor_callbacks_human_phase/src/unit_01416.c"
#include "../artifacts/actor_callbacks_human_phase/src/unit_01427.c"
#include "../artifacts/actor_callbacks_human_phase/runtime/actor_callbacks_human_boundary.c"
#endif
#undef FUN_004b1d50
#undef FUN_004b3250
#undef restore_actor_human_boundary
#include "../src/unit_01097.c"
