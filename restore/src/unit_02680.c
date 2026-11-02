#include "restore_types.h"
extern longlong FUN_0056133c();

void FUN_0055d9c0(uint *param_1,int param_2,float param_3)

{
  int iVar1;
  float fVar2;
  float fVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  uint local_8;
  
  uVar4 = (undefined2)(local_8 >> 0x10);
  local_8 = local_8 & 0xffff0000;
  if (*(byte *)(param_2 + 0xb) < 2) {
    *param_1 = local_8;
    param_1[1] = (uint)(180.0 <= param_3);
    return;
  }
  if ((*(byte *)(param_2 + 1) & 0x80) != 0) {
    iVar1 = (uint)*(byte *)(param_2 + 0xb) * 2 + -2;
    for (fVar2 = (360.0 - (param_3 - 135.0)) + (float)(int)(0x168 / (longlong)(iVar1 * 2));
        fVar2 < 0.0; fVar2 = fVar2 + 360.0) {
    }
    for (; 360.0 <= fVar2; fVar2 = fVar2 - 360.0) {
    }
    fVar3 = (float)iVar1;
    if (fVar2 / (360.0 / fVar3) <= fVar3 * 0.5) {
      uVar5 = FUN_0056133c();
      *param_1 = CONCAT22(uVar4,uVar5);
      param_1[1] = 0;
      return;
    }
    uVar5 = FUN_0056133c();
    *param_1 = CONCAT22(uVar4,uVar5);
    param_1[1] = 1;
    return;
  }
  uVar5 = FUN_0056133c();
  *param_1 = CONCAT22(uVar4,uVar5);
  param_1[1] = 0;
  return;
}
