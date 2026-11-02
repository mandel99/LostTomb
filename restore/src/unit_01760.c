#include "restore_types.h"
extern undefined4 FUN_005647eb();
extern uint FUN_00568eab();
extern int FUN_0056b1b6();

undefined4 FUN_004ced50(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_00568eab(param_1,0x8000);
  if (iVar1 == -1) {
    return 0;
  }
  uVar2 = FUN_0056b1b6(iVar1);
  FUN_005647eb(iVar1);
  return uVar2;
}
