#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00f56d74 (*(undefined *)0x00f56d74u)
extern void FUN_0041f910();
extern int FUN_0053e590();

void FUN_004e5e90(int param_1,int param_2,int param_3)

{
  byte bVar1;
  short sVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  float local_18;
  int local_c;
  int local_8;
  int local_4;
  
  iVar4 = param_1;
  uVar3 = DAT_00f56d74;
  bVar1 = *(byte *)((int)&DAT_00799a2c + param_1 * 0x108 + 1);
  if (99 < bVar1) {
    return;
  }
  uVar8 = 0;
  param_1 = 0;
  if (bVar1 < 0x42) {
    if (bVar1 < 0x21) goto LAB_004e5eeb;
    param_1 = (uint)bVar1 * 3 + -99;
  }
  else {
    param_1 = (100 - (uint)bVar1) * 3;
  }
  if (99 < param_1) {
    param_1 = 100;
  }
LAB_004e5eeb: ;
  sVar2 = (&DAT_007999e8)[iVar4 * 0x84];
  if (sVar2 == 0xcf) {
    iVar5 = 0x5a;
    iVar7 = 0x12;
  }
  else {
    iVar5 = 0x98;
    iVar7 = 0xe;
  }
  uVar9 = (-(uint)(sVar2 != 0xcf) & 5) + 1;
  local_18 = 0.0;
  if (0.0 < (float)(int)param_1) {
    do {
      uVar6 = uVar9;
      if ((uVar8 & 1) == 0) {
        uVar6 = -uVar9 - 0x3c;
      }
      FUN_0053e590(uVar3,uVar6 + param_2 + iVar5,
                   (((int)uVar8 / 2 + 1) * -0x3c - uVar9 / 2) + param_3 + iVar7);
      local_18 = local_18 + (100.0 / (float)(byte)((-(sVar2 != 0xcf) & 2U) + 2)) * 0.5;
      uVar8 = uVar8 + 1;
    } while (local_18 < (float)(int)param_1);
  }
  for (iVar10 = 0; iVar10 < (int)((-(uint)((&DAT_007999e8)[iVar4 * 0x84] != 0xcf) & 8) + 10);
      iVar10 = iVar10 + 1) {
    FUN_0041f910(&local_4,&local_8,&local_c);
    if (local_c != 0) {
      FUN_0053e590(local_c,local_4 + param_2 + iVar5,local_8 + param_3 + iVar7);
    }
  }
  return;
}
