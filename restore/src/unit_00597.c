#include "restore_types.h"
#define DAT_00775614 (*(undefined4 *)0x00775614u)
#define DAT_00775618 (*(undefined4 *)0x00775618u)
#define DAT_00775628 (*(undefined4 *)0x00775628u)
#define DAT_0077562c (*(undefined4 *)0x0077562cu)
extern undefined4 FUN_00441040();

uint FUN_00441130(int param_1,int param_2,int *param_3,int *param_4,int param_5,int param_6)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  int local_90;
  uint local_8c;
  int local_88;
  int local_84;
  int local_80 [32];
  
  iVar4 = 0;
  local_90 = 0;
  if (param_5 == 0x1f) {
    local_90 = 2;
  }
  else if (param_5 == 0x1e) {
    local_90 = 3;
  }
  else if (param_5 == 0x21) {
    local_90 = 4;
  }
  else if (param_5 == 0xd1) {
    local_90 = 5;
  }
  uVar5 = 1;
  uVar6 = 1;
  if (param_6 == 0x1e) {
    uVar6 = 2;
    local_80[0] = 1;
    local_80[1] = 1;
  }
  else if (param_6 == 0x1f) {
    uVar6 = 1;
    local_80[0] = 0;
    local_80[1] = 0;
  }
  else if (param_6 == 0x21) {
    uVar5 = 2;
    uVar6 = 2;
    local_80[0] = 2;
    local_80[1] = 2;
  }
  local_8c = 0;
  local_84 = 5;
  FUN_00441040(local_90,uVar5,uVar6,&local_88,local_80);
  if (local_88 < 1) {
    if (param_6 == 0x1e) {
      FUN_00441040(local_90,uVar6,uVar5,&local_88,local_80);
    }
    local_8c = (uint)(param_6 == 0x1e);
    if (local_88 < 1) {
      *param_3 = local_80[0] + param_1;
      *param_4 = local_80[1] + param_2;
      return 0xffffffff;
    }
  }
  iVar3 = 0;
  if (0 < local_88) {
    piVar2 = local_80 + 1;
    do {
      if (local_90 == 2) break;
      if (local_90 == 3) {
        iVar1 = *(int *)((piVar2[-1] * 3 + *piVar2) * 4 + 0x5d2960);
      }
      else if (local_90 == 4) {
        iVar1 = *(int *)((*piVar2 + piVar2[-1] * 4) * 4 + 0x5d2984);
      }
      else {
        iVar1 = 0;
      }
      if (iVar1 < local_84) {
        iVar4 = iVar3;
        local_84 = iVar1;
      }
      iVar3 = iVar3 + 1;
      piVar2 = piVar2 + 2;
    } while (iVar3 < local_88);
  }
  iVar3 = local_80[iVar4 * 2];
  iVar4 = local_80[iVar4 * 2 + 1];
  *param_3 = param_1 + iVar3;
  *param_4 = param_2 + iVar4;
  if (param_6 == 0x1e) {
    if (local_8c != 0) {
      iVar3 = iVar3 + iVar4 + iVar3 * 4;
      (&DAT_00775614)[iVar3] = 1;
      (&DAT_00775628)[iVar3] = 1;
      return local_8c;
    }
    iVar3 = iVar3 + iVar4 + iVar3 * 4;
    (&DAT_00775614)[iVar3] = 1;
    (&DAT_00775618)[iVar3] = 1;
    return 0;
  }
  if ((param_6 != 0x1f) && (param_6 == 0x21)) {
    iVar3 = iVar3 + iVar4 + iVar3 * 4;
    (&DAT_00775614)[iVar3] = 1;
    (&DAT_00775628)[iVar3] = 1;
    (&DAT_00775618)[iVar3] = 1;
    (&DAT_0077562c)[iVar3] = 1;
    return local_8c;
  }
  (&DAT_00775614)[iVar3 + iVar4 + iVar3 * 4] = 1;
  return local_8c;
}
