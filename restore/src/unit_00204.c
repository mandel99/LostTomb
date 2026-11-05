#include "native_member_bridge.h"
#include "actor_laborer.h"
/* Reuse existing laborer C with actual native GPR/flags/stack; keep live x87. */
RESTORE_NATIVE_MEMBER_ENTRY(FUN_00417640,restore_laborer_native_00417640,0x00417640,4,restore_actor_laborer_raw)
