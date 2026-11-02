#include "restore_types.h"

void FUN_0055cb10(int param_1,byte *param_2,byte *param_3)

{
  byte *pbVar1;
  ushort *puVar2;
  byte bVar3;
  byte *pbVar4;
  ushort uVar5;
  uint uVar6;
  short *psVar7;
  short sVar8;
  short *psVar9;
  
  psVar7 = (short *)(param_1 + 0xdc8);
  sVar8 = 0;
  uVar6 = (param_1 - (int)psVar7) + 0x1fcbU >> 2;
  psVar9 = psVar7;
  if (uVar6 < 2) {
    uVar6 = 1;
  }
  for (; pbVar4 = param_2, uVar6 != 0; uVar6 = uVar6 - 1) {
    psVar9[0] = 0;
    psVar9[1] = 0;
    psVar9 = psVar9 + 2;
  }
  do {
    pbVar1 = pbVar4 + 1;
    psVar9 = (short *)((uint)*pbVar1 * 10 + (uint)*pbVar4 * 8 + 0xdc8 + param_1);
    *psVar9 = *psVar9 + 1;
    pbVar4 = pbVar1;
  } while (pbVar1 < param_3);
  do {
    sVar8 = sVar8 + *psVar7;
    psVar9 = psVar7 + 1;
    *psVar7 = sVar8;
    psVar7 = psVar9;
  } while (psVar9 < (short *)(param_1 + 0x1fc8));
  param_3 = param_3 + -1;
  do {
    pbVar4 = param_3 + 1;
    bVar3 = *param_3;
    sVar8 = (short)param_3;
    param_3 = param_3 + -1;
    puVar2 = (ushort *)((uint)bVar3 * 8 + (uint)*pbVar4 * 10 + 0xdc8 + param_1);
    uVar5 = *puVar2 - 1;
    *puVar2 = uVar5;
    *(short *)(param_1 + 0x49d0 + (uint)uVar5 * 2) = (sVar8 - (short)param_1) + -0x27cc;
  } while (param_2 <= param_3);
  return;
}
