/* Exact candidate/applied bodies; observation wrappers are private. */
#define RM_PUBLIC(name) RM_BODY_##name
#define RM_BODY_FUN_00418510 replay_plain_00418510
#define RM_BODY_FUN_004181e0 replay_plain_004181e0
#include "replay_mission.h"
#ifdef REPLAY_MISSION_STAGED
#include "../runtime/replay_mission.c"
#include "../src/unit_00222.c"
#else
#include "../../../runtime/replay_mission.c"
#include "../../../src/unit_00222.c"
#endif
#undef RM_PUBLIC
#undef RM_BODY_FUN_00418510
#undef RM_BODY_FUN_004181e0
#define FUN_005169c0 replay_plain_005169c0
#include "../../../src/unit_02282.c"
#undef FUN_005169c0
