#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)

void FUN_00547b60(int param_1)

{
  int iVar1;
  int *in_ECX;
  
  param_1 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  iVar1 = (**(code **)(*in_ECX + 0x38))();
  iVar1 = iVar1 + param_1;
  for (; param_1 < iVar1; param_1 = param_1 + 1) {
    (**(code **)(*in_ECX + 0x18))(param_1);
  }
  return;
}
