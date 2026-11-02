#include "restore_types.h"

void FUN_00455540(int param_1,uint param_2,uint param_3)

{
  uint *puVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = 0;
  if (0 < (int)(param_2 - 7)) {
    uVar2 = param_2 >> 3;
    iVar3 = uVar2 * 8;
    puVar1 = (uint *)(param_1 + 8);
    do {
      puVar1[-2] = puVar1[-2] & param_3;
      puVar1[-1] = puVar1[-1] & param_3;
      *puVar1 = *puVar1 & param_3;
      puVar1[1] = puVar1[1] & param_3;
      puVar1[2] = puVar1[2] & param_3;
      puVar1[3] = puVar1[3] & param_3;
      puVar1[4] = puVar1[4] & param_3;
      uVar2 = uVar2 - 1;
      puVar1[5] = puVar1[5] & param_3;
      puVar1 = puVar1 + 8;
    } while (uVar2 != 0);
  }
  if (iVar3 < (int)param_2) {
    iVar4 = param_2 - iVar3;
    puVar1 = (uint *)(param_1 + iVar3 * 4);
    do {
      iVar4 = iVar4 + -1;
      *puVar1 = *puVar1 & param_3;
      puVar1 = puVar1 + 1;
    } while (iVar4 != 0);
  }
  return;
}
