#include "restore_types.h"
extern undefined4 * FUN_00563400();

int FUN_00560ff9(undefined1 *param_1,int param_2)

{
  int iVar1;
  int *in_ECX;
  int local_8;
  
  local_8 = 0;
  if (in_ECX[2] == 0) {
    while ((param_2 != 0 && (iVar1 = (**(code **)(*in_ECX + 0x20))(), iVar1 != -1))) {
      iVar1 = in_ECX[0xb] - in_ECX[10];
      if (param_2 <= iVar1) {
        iVar1 = param_2;
      }
      if (0 < iVar1) {
        FUN_00563400(param_1,in_ECX[10],iVar1);
        param_1 = param_1 + iVar1;
        in_ECX[10] = in_ECX[10] + iVar1;
        local_8 = local_8 + iVar1;
        param_2 = param_2 - iVar1;
      }
    }
  }
  else {
    if (in_ECX[3] == -1) {
      iVar1 = (**(code **)(*in_ECX + 0x20))();
      in_ECX[3] = iVar1;
    }
    while ((param_2 != 0 && (param_2 = param_2 + -1, in_ECX[3] != -1))) {
      *param_1 = (char)in_ECX[3];
      param_1 = param_1 + 1;
      local_8 = local_8 + 1;
      iVar1 = (**(code **)(*in_ECX + 0x20))();
      in_ECX[3] = iVar1;
    }
  }
  return local_8;
}
