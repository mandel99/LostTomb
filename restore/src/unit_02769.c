#include "restore_types.h"
extern int FUN_00561178();
extern undefined4 * FUN_00562880();

undefined4 FUN_0056111c(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  int *in_ECX;
  
  if ((uint)in_ECX[9] < (uint)in_ECX[10]) {
    uVar1 = FUN_00561178(param_1);
  }
  else {
    uVar1 = 1;
    iVar2 = (**(code **)(*in_ECX + 0xc))(0xffffffff,1);
    if (iVar2 == -1) {
      uVar1 = 0xffffffff;
    }
    else if ((in_ECX[2] == 0) && (in_ECX[0xb] != 0)) {
      iVar2 = in_ECX[10];
      FUN_00562880(iVar2 + 1,iVar2,(in_ECX[0xb] - iVar2) + -1);
      *(char *)in_ECX[10] = (char)uVar1;
    }
  }
  return uVar1;
}
