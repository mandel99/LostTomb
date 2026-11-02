#include "restore_types.h"
#define DAT_005d2914 (*(undefined4 *)0x005d2914u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)

uint FUN_0043f250(uint param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  uint local_20 [4];
  uint auStack_10 [4];
  
  iVar6 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  if ((*(byte *)(&DAT_00af598c + iVar6) & 0x40) != 0) {
    local_20[0] = param_2 - param_4;
    local_20[3] = param_1 - param_3;
    local_20[2] = (param_4 - param_2) + -1 + param_5;
    local_20[1] = param_5 + -1 + (param_3 - param_1);
    iVar5 = 0;
    iVar7 = 0;
    iVar8 = 0;
    param_4 = 0;
    param_1 = 0;
    param_2 = 0;
    do {
      iVar1 = (&DAT_005d2914)[iVar5];
      auStack_10[iVar5] = (&DAT_00af598c)[iVar1 + iVar6] & 0x40;
      uVar2 = local_20[iVar5];
      if (1 < (int)uVar2) {
        iVar9 = uVar2 - 1;
        puVar4 = &DAT_00af598c + iVar6 + iVar1 * 2;
        do {
          uVar3 = *puVar4;
          puVar4 = puVar4 + iVar1;
          auStack_10[iVar5] = auStack_10[iVar5] & uVar3 & 0x40;
          iVar9 = iVar9 + -1;
          iVar7 = param_4;
          iVar8 = param_1;
        } while (iVar9 != 0);
      }
      uVar3 = auStack_10[iVar5];
      iVar1 = iVar5;
      if ((uVar3 != 0) && (iVar8 = iVar8 + 1, param_1 = iVar8, iVar1 = param_2, uVar2 != 0)) {
        iVar7 = iVar7 + 1;
        param_4 = iVar7;
      }
      param_2 = iVar1;
      iVar5 = iVar5 + 1;
    } while (iVar5 < 4);
    param_1 = uVar3;
    if ((((param_5 == 2) && (2 < iVar8)) && (1 < iVar7)) &&
       ((param_1 = local_20[param_2], param_1 == 0 || (iVar8 == 4)))) {
      return CONCAT31((int3)(param_1 >> 8),1);
    }
    if ((3 < iVar8) && (2 < iVar7)) {
      return CONCAT31((int3)(param_1 >> 8),1);
    }
    if ((((param_5 != 5) && (iVar8 == 3)) && (iVar7 == 3)) &&
       (param_1 = local_20[param_2], param_1 == 0)) {
      return 1;
    }
  }
  return param_1 & 0xffffff00;
}
