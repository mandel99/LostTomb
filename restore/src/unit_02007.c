#include "native_member_bridge.h"
#include "actor_laborer.h"
/* Reuse existing laborer C with actual native GPR/flags/stack; keep live x87. */
RESTORE_NATIVE_MEMBER_ENTRY(FUN_004e6e70,restore_laborer_native_004e6e70,0x004e6e70,12,restore_actor_laborer_raw)
