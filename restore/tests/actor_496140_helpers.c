#include "actor_callback_496140.h"
#define restore_actor_496140_queue_contains restore_496140_queue_body
#ifdef RESTORE_ACTOR_496140_PRODUCTION
#include "../runtime/actor_callback_496140_queue.c"
#else
#include "../artifacts/actor_callback_496140_phase/runtime/actor_callback_496140_queue.c"
#endif
#undef restore_actor_496140_queue_contains
#define FUN_00496060 restore_496140_shop_body
#define FUN_004a35e0 restore_496140_wait_body
#ifdef RESTORE_ACTOR_496140_PRODUCTION
#include "../src/unit_01335.c"
#include "../src/unit_01358.c"
#else
#include "../artifacts/actor_callback_496140_phase/src/unit_01335.c"
#include "../artifacts/actor_callback_496140_phase/src/unit_01358.c"
#endif
#undef FUN_00496060
#undef FUN_004a35e0
#define FUN_004b1d50 restore_496140_timer_body
#define FUN_004b3250 restore_496140_route_body
#ifdef RESTORE_ACTOR_496140_PRODUCTION
#include "../src/unit_01416.c"
#include "../src/unit_01427.c"
#else
#include "../artifacts/actor_callbacks_human_phase/src/unit_01416.c"
#include "../artifacts/actor_callbacks_human_phase/src/unit_01427.c"
#endif
#undef FUN_004b1d50
#undef FUN_004b3250
