#include "native_member_bridge.h"
#include "actor_laborer.h"
/* Reuse existing laborer C with actual native GPR/flags/stack; keep live x87. */
RESTORE_NATIVE_MEMBER_ENTRY(FUN_004246b0,restore_laborer_native_004246b0,0x004246b0,0,restore_actor_laborer_raw)
