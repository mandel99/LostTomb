#include "restore_types.h"
extern undefined4 FUN_005647eb();
extern DWORD FUN_005662e3();
extern int FUN_005664d5();
extern uint FUN_00568eab();

undefined4 FUN_004ce920(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar1 = FUN_00568eab(param_1,0x8101,0x180);
  if (iVar1 == -1) {
    return 0;
  }
  iVar2 = FUN_005662e3(iVar1,param_4,0);
  if (iVar2 == -1) {
    return 0;
  }
  uVar3 = FUN_005664d5(iVar1,param_2,param_3);
  FUN_005647eb(iVar1);
  return uVar3;
}
