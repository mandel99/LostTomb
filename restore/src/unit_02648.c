#include "restore_types.h"
extern void FUN_0055ca90();

void FUN_0055c990(int param_1,uint param_2,uint param_3)

{
  byte *pbVar1;
  int iVar2;
  byte bVar3;
  
  if (8 < param_2) {
    param_2 = param_2 - 8;
    FUN_0055c990(param_1,8,param_3);
    param_3 = param_3 >> 8;
  }
  bVar3 = (byte)*(int *)(param_1 + 8);
  param_2 = *(int *)(param_1 + 8) + param_2;
  pbVar1 = (byte *)(*(int *)(param_1 + 4) + 0x1fca + param_1);
  *pbVar1 = *pbVar1 | (char)param_3 << (bVar3 & 0x1f);
  *(uint *)(param_1 + 8) = param_2;
  if (param_2 < 9) {
    *(uint *)(param_1 + 8) = param_2 & 7;
    if ((param_2 & 7) == 0) {
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
    }
  }
  else {
    iVar2 = *(int *)(param_1 + 4);
    *(int *)(param_1 + 4) = iVar2 + 1;
    *(char *)(iVar2 + 0x1fcb + param_1) = (char)(param_3 >> (8 - bVar3 & 0x1f));
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 7;
  }
  if (0x7ff < *(uint *)(param_1 + 4)) {
    FUN_0055ca90(param_1);
  }
  return;
}
