#include "native_member_bridge.h"
#include "actor_laborer.h"
/* Reuse existing laborer C with actual native GPR/flags/stack; keep live x87. */
RESTORE_NATIVE_MEMBER_ENTRY(FUN_005590a0,restore_laborer_native_005590a0,0x005590a0,4,restore_actor_laborer_raw)
