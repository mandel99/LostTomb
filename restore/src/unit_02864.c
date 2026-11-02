#include "restore_types.h"
#define LAB_00561a90 (*(undefined1 *)0x00561a90u)
#define DAT_00570678 (*(undefined4 *)0x00570678u)
#define DAT_01266418 (*(undefined4 *)0x01266418u)
#define DAT_0126641c (*(undefined4 *)0x0126641cu)
extern undefined4 FUN_0056158a();
extern void FUN_00563dd6();

undefined4
FUN_00563d10(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  undefined4 uVar1;
  void *local_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_00570678;
  puStack_10 = &LAB_00561a90;
  local_14 = ExceptionList;
  DAT_01266418 = param_1;
  DAT_0126641c = param_3;
  local_8 = 1;
  ExceptionList = &local_14;
  uVar1 = FUN_0056158a(param_2,param_4,param_5,param_6,param_7);
  local_8 = 0xffffffff;
  FUN_00563dd6();
  ExceptionList = local_14;
  return uVar1;
}
