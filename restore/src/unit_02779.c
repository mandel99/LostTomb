#include "restore_types.h"
#define LAB_00561a90 (*(undefined1 *)0x00561a90u)
#define DAT_00570610 (*(undefined4 *)0x00570610u)
extern void FUN_005614ab();

void FUN_00561441(undefined4 param_1,undefined4 param_2,int param_3,code *param_4)

{
  int local_20;
  void *local_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_00570610;
  puStack_10 = &LAB_00561a90;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  for (local_20 = 0; local_20 < param_3; local_20 = local_20 + 1) {
    (*param_4)();
  }
  local_8 = 0xffffffff;
  FUN_005614ab();
  ExceptionList = local_14;
  return;
}
