/* Exact staged/applied query bodies, with private observation aliases only. */
#define TQ_PUBLIC(name) TQ_BODY_##name
#define TQ_BODY_FUN_00443950 queries_actual_00443950
#define TQ_BODY_FUN_004439c0 queries_actual_004439c0
#define TQ_BODY_FUN_00443a20 queries_actual_00443a20
#define TQ_BODY_FUN_00443b20 queries_actual_00443b20
#define TQ_BODY_FUN_00443b90 queries_actual_00443b90
#define TQ_BODY_FUN_00443d90 queries_actual_00443d90
#define TQ_BODY_FUN_00447790 queries_actual_00447790
#define TQ_BODY_FUN_004c5190 queries_actual_004c5190
#include "training_queries.h"
#ifdef TRAINING_QUERIES_STAGED
#include "../src/unit_00625.c"
#else
#include "../../../src/unit_00625.c"
#endif
#ifdef TRAINING_QUERIES_STAGED
#include "../src/unit_00626.c"
#else
#include "../../../src/unit_00626.c"
#endif
#ifdef TRAINING_QUERIES_STAGED
#include "../src/unit_00627.c"
#else
#include "../../../src/unit_00627.c"
#endif
#ifdef TRAINING_QUERIES_STAGED
#include "../src/unit_00630.c"
#else
#include "../../../src/unit_00630.c"
#endif
#ifdef TRAINING_QUERIES_STAGED
#include "../src/unit_00631.c"
#else
#include "../../../src/unit_00631.c"
#endif
#ifdef TRAINING_QUERIES_STAGED
#include "../src/unit_00632.c"
#else
#include "../../../src/unit_00632.c"
#endif
#ifdef TRAINING_QUERIES_STAGED
#include "../src/unit_00669.c"
#else
#include "../../../src/unit_00669.c"
#endif
#ifdef TRAINING_QUERIES_STAGED
#include "../src/unit_01603.c"
#else
#include "../../../src/unit_01603.c"
#endif
#undef TQ_PUBLIC
#undef TQ_BODY_FUN_00443950
#undef TQ_BODY_FUN_004439c0
#undef TQ_BODY_FUN_00443a20
#undef TQ_BODY_FUN_00443b20
#undef TQ_BODY_FUN_00443b90
#undef TQ_BODY_FUN_00443d90
#undef TQ_BODY_FUN_00447790
#undef TQ_BODY_FUN_004c5190
#define FUN_004c5240 queries_actual_004c5240
#include "../../../src/unit_01605.c"
#undef FUN_004c5240
