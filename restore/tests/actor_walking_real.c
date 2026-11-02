#include "actor_walking.h"
#define FUN_004a5da0 actor_walking_actual_schedule
#ifdef ACTOR_WALKING_STAGED
#include "../artifacts/actor_walking_phase/unit_01387.c"
#else
#include "../src/unit_01387.c"
#endif
#undef FUN_004a5da0
#define FUN_004b27e0 actor_walking_actual_step
#ifdef ACTOR_WALKING_STAGED
#include "../artifacts/actor_walking_phase/unit_01423.c"
#else
#include "../src/unit_01423.c"
#endif
#undef FUN_004b27e0
#define FUN_004b33d0 actor_walking_actual_direction
#ifdef ACTOR_WALKING_STAGED
#include "../artifacts/actor_walking_phase/unit_01431.c"
#else
#include "../src/unit_01431.c"
#endif
#undef FUN_004b33d0
#define FUN_004b3530 actor_walking_actual_gate
#ifdef ACTOR_WALKING_STAGED
#include "../artifacts/actor_walking_phase/unit_01433.c"
#else
#include "../src/unit_01433.c"
#endif
#undef FUN_004b3530
#define FUN_004b3c60 actor_walking_actual_commit
#ifdef ACTOR_WALKING_STAGED
#include "../artifacts/actor_walking_phase/unit_01434.c"
#else
#include "../src/unit_01434.c"
#endif
#undef FUN_004b3c60
#define FUN_004b52f0 actor_walking_actual_link
#ifdef ACTOR_WALKING_STAGED
#include "../artifacts/actor_walking_phase/unit_01451.c"
#else
#include "../src/unit_01451.c"
#endif
#undef FUN_004b52f0
#define FUN_004b5420 actor_walking_actual_unlink
#ifdef ACTOR_WALKING_STAGED
#include "../artifacts/actor_walking_phase/unit_01453.c"
#else
#include "../src/unit_01453.c"
#endif
#undef FUN_004b5420
#define FUN_004b3f00 actor_walking_actual_pixel
#ifdef ACTOR_WALKING_STAGED
#include "../artifacts/actor_combat_phase/src/unit_01435.c"
#else
#include "../src/unit_01435.c"
#endif
#undef FUN_004b3f00
#define FUN_004b3250 actor_walking_actual_release
#include "../src/unit_01427.c"
#undef FUN_004b3250
#define FUN_0051cba0 actor_walking_actual_heading
#include "../src/unit_02332.c"
#undef FUN_0051cba0
#include "../src/unit_02351.c"
#define restore_actor_walking_service actor_walking_actual_service
#define restore_actor_walking_empty actor_walking_actual_empty
#include "../runtime/actor_walking_facades.c"
#undef restore_actor_walking_service
#undef restore_actor_walking_empty
#define restore_actor_walking_queue_count actor_walking_actual_queue_count
#include "../runtime/actor_walking_queue_count.c"
#undef restore_actor_walking_queue_count
#define restore_actor_walking_queue_full actor_walking_actual_queue_full
#include "../runtime/actor_walking_queue_full.c"
#undef restore_actor_walking_queue_full
#define restore_actor_walking_queue_push actor_walking_actual_queue_push
#include "../runtime/actor_walking_queue_push.c"
#undef restore_actor_walking_queue_push
