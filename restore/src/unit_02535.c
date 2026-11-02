#include "restore_types.h"

void FUN_00540570(undefined4 param_1,int *param_2,int param_3)

{
  int iVar1;
  
  iVar1 = param_2[2];
  while (iVar1 < param_3) {
    param_2 = (int *)*param_2;
    iVar1 = param_2[2];
  }
  return;
}
