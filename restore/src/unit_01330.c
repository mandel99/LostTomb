#include "restore_types.h"

int FUN_00495da0(int *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = 0;
  iVar1 = -1;
  iVar3 = 0;
  do {
    if (iVar2 < *param_1 / 100) {
      iVar1 = iVar3;
      iVar2 = *param_1 / 100;
    }
    iVar3 = iVar3 + 1;
    param_1 = param_1 + 1;
  } while (iVar3 < 4);
  return iVar1;
}
