#include "native_member_bridge.h"
#include "actor_laborer.h"
/* Reuse existing laborer C with actual native GPR/flags/stack; keep live x87. */
RESTORE_NATIVE_MEMBER_ENTRY(FUN_00547870,restore_laborer_native_00547870,0x00547870,8,restore_actor_laborer_raw)
