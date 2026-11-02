#include "restore_types.h"
#define LAB_0056b738 (*(undefined1 *)0x0056b738u)
extern void FUN_004f66c0();
extern void FUN_004f66f0();
extern undefined4 FUN_004f6700();
extern void FUN_004f6790();

undefined4 FUN_004f6a50(int param_1)

{
  int in_ECX;
  undefined1 local_28 [28];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_0056b738;
  local_c = ExceptionList;
  if (*(int *)(in_ECX + 0x10) < param_1) {
    ExceptionList = &local_c;
    FUN_004f66c0(in_ECX);
    local_4 = 0;
    FUN_004f6700(param_1);
    FUN_004f6790(local_28);
    local_4 = 0xffffffff;
    FUN_004f66f0();
  }
  ExceptionList = local_c;
  return 1;
}
