#include "restore_types.h"
#define LAB_00561a90 (*(undefined1 *)0x00561a90u)
#define DAT_005705f0 (*(undefined4 *)0x005705f0u)
extern void FUN_005613cb();

void FUN_00561363(undefined4 param_1,undefined4 param_2,int param_3,code *param_4)

{
  void *local_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_005705f0;
  puStack_10 = &LAB_00561a90;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  while( true ) {
    param_3 = param_3 + -1;
    if (param_3 < 0) break;
    (*param_4)();
  }
  local_8 = 0xffffffff;
  FUN_005613cb();
  ExceptionList = local_14;
  return;
}
