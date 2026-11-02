#include "placement_preview.h"
#define FUN_004f0ba0 restore_placement_project
#define FUN_0053f4f0 restore_placement_mask_global
#define restore_placement_predicate restore_placement_predicate_body
#ifdef RESTORE_PLACEMENT_PREVIEW_PRODUCTION
#include "../src/unit_02115.c"
#include "../src/unit_02527.c"
#include "../runtime/placement_preview_predicates.c"
#else
#include "../artifacts/placement_preview_phase/src/unit_02115.c"
#include "../artifacts/placement_preview_phase/src/unit_02527.c"
#include "../artifacts/placement_preview_phase/runtime/placement_preview_predicates.c"
#endif
#undef FUN_004f0ba0
#undef FUN_0053f4f0
#undef restore_placement_predicate
#define restore_placement_member_adapter restore_placement_member_adapter_body
#ifdef RESTORE_PLACEMENT_PREVIEW_PRODUCTION
#include "../runtime/placement_preview_members.c"
#else
#include "../artifacts/placement_preview_phase/runtime/placement_preview_members.c"
#endif
#undef restore_placement_member_adapter
