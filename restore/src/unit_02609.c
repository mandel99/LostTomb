#include "native_member_bridge.h"
#include "actor_laborer.h"
/* Reuse existing laborer C with actual native GPR/flags/stack; keep live x87. */
RESTORE_NATIVE_MEMBER_ENTRY(FUN_00555e80,restore_laborer_native_00555e80,0x00555e80,8,restore_actor_laborer_raw)
