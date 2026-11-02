#include "restore_types.h"
#define DAT_005e058c (*(undefined4 *)0x005e058cu)

int FUN_004cc070(byte *param_1)

{
  byte *pbVar1;
  byte bVar2;
  byte bVar3;
  byte *pbVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  
  bVar2 = *param_1;
  iVar6 = 0;
  iVar7 = 0;
  pbVar4 = param_1;
  if (bVar2 == 0x2d) {
    pbVar4 = param_1 + 1;
  }
  bVar3 = *pbVar4;
  while ((0x2f < bVar3 && (bVar3 < 0x3a))) {
    pbVar1 = pbVar4 + 1;
    iVar7 = iVar7 + 1;
    pbVar4 = pbVar4 + 1;
    bVar3 = *pbVar1;
  }
  if (bVar2 == 0x2d) {
    param_1 = param_1 + 1;
  }
  if (iVar7 != 0) {
    piVar5 = &DAT_005e058c + iVar7;
    do {
      piVar5 = piVar5 + -1;
      iVar6 = iVar6 + (*param_1 - 0x30) * *piVar5;
      param_1 = param_1 + 1;
      iVar7 = iVar7 + -1;
    } while (iVar7 != 0);
  }
  if (bVar2 == 0x2d) {
    iVar6 = -iVar6;
  }
  return iVar6;
}
