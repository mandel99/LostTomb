#include "restore_types.h"
extern uint FUN_004f3d70();

undefined4 FUN_004f4420(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = FUN_004f3d70(param_1);
  if ((1 < iVar1) && (iVar1 < 0xd)) {
    return 0x32;
  }
  return 0;
}
