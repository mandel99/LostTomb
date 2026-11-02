#include "restore_types.h"

void FUN_0055ca20(uint *param_1,byte param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint *puVar5;
  
  uVar2 = (uint)param_2;
  uVar3 = param_3 >> 2;
  if (uVar3 != 0) {
    puVar5 = param_1;
    uVar4 = uVar3;
    if (uVar3 < 2) {
      uVar3 = 1;
      uVar4 = uVar3;
    }
    for (; uVar3 != 0; uVar3 = uVar3 - 1) {
      *puVar5 = (uVar2 | uVar2 << 0x10) << 8 | uVar2 << 0x10 | uVar2;
      puVar5 = puVar5 + 1;
    }
    param_1 = param_1 + uVar4;
  }
  param_3 = param_3 & 3;
  if (param_3 != 0) {
    if (param_3 < 2) {
      param_3 = 1;
    }
    for (iVar1 = 0; iVar1 != 0; iVar1 = iVar1 + -1) {
      *param_1 = CONCAT22(CONCAT11(param_2,param_2),CONCAT11(param_2,param_2));
      param_1 = param_1 + 1;
    }
    for (; param_3 != 0; param_3 = param_3 - 1) {
      *(byte *)param_1 = param_2;
      param_1 = (uint *)((int)param_1 + 1);
    }
  }
  return;
}
