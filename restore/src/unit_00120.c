#include "restore_types.h"

void FUN_0040f920(uint *param_1,int param_2,byte param_3)

{
  uint uVar1;
  
  uVar1 = CONCAT31(CONCAT21(CONCAT11(param_3,param_3),param_3),param_3);
  for (; 0x3f < param_2; param_2 = param_2 + -0x40) {
    *param_1 = *param_1 & uVar1;
    param_1[1] = param_1[1] & uVar1;
    param_1[2] = param_1[2] & uVar1;
    param_1[3] = param_1[3] & uVar1;
    param_1[4] = param_1[4] & uVar1;
    param_1[5] = param_1[5] & uVar1;
    param_1[6] = param_1[6] & uVar1;
    param_1[7] = param_1[7] & uVar1;
    param_1[8] = param_1[8] & uVar1;
    param_1[9] = param_1[9] & uVar1;
    param_1[10] = param_1[10] & uVar1;
    param_1[0xb] = param_1[0xb] & uVar1;
    param_1[0xc] = param_1[0xc] & uVar1;
    param_1[0xd] = param_1[0xd] & uVar1;
    param_1[0xe] = param_1[0xe] & uVar1;
    param_1[0xf] = param_1[0xf] & uVar1;
    param_1 = param_1 + 0x10;
  }
  for (; 0 < param_2; param_2 = param_2 + -1) {
    *(byte *)param_1 = (byte)*param_1 & param_3;
    param_1 = (uint *)((int)param_1 + 1);
  }
  return;
}
