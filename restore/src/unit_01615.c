#include "restore_types.h"
#define DAT_0057c280 (*(undefined4 *)0x0057c280u)
extern undefined4 FUN_00561994();
extern undefined4 * FUN_00561a0a();
extern uint FUN_00561f5f();

bool FUN_004c58d0(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_00561a0a(param_1,&DAT_0057c280);
  if (iVar1 == 0) {
    return false;
  }
  iVar2 = FUN_00561f5f();
  FUN_00561994(iVar1);
  return iVar2 == 1;
}
