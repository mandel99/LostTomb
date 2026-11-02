#include "restore_types.h"
extern undefined4 FUN_005647eb();
extern int FUN_005664d5();
extern uint FUN_00568eab();

undefined4 FUN_004ced90(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = FUN_00568eab(param_1,0x8301,0x180);
  if (iVar1 == -1) {
    return 0;
  }
  FUN_005664d5(iVar1,param_2,param_3);
  FUN_005647eb(iVar1);
  return 1;
}
