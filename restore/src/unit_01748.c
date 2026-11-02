#include "restore_types.h"
extern void FUN_004ce2d0();
extern undefined4 FUN_005647eb();
extern uint FUN_00568eab();

undefined4 FUN_004ce450(undefined4 param_1)

{
  int iVar1;
  
  FUN_004ce2d0();
  iVar1 = FUN_00568eab(param_1,0x8000);
  if (-1 < iVar1) {
    FUN_005647eb(iVar1);
    return 1;
  }
  return 0;
}
