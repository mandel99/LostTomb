#include "restore_types.h"
#define DAT_005df9bc (*(undefined4 *)0x005df9bcu)
#define DAT_005e00cc (*(undefined4 *)0x005e00ccu)
#define DAT_005e00d0 (*(undefined4 *)0x005e00d0u)
#define DAT_005e014c (*(undefined4 *)0x005e014cu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
extern void FUN_004746d0();
extern void FUN_004c0860();

void FUN_004c0be0(int param_1)

{
  uint uVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int local_24;
  int local_20;
  int local_18;
  
  uVar1 = (uint)*(byte *)(&DAT_00799a82 + param_1 * 0x84);
  iVar6 = (int)uVar1 >> 1;
  iVar8 = (int)*(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2) +
          *(int *)(&DAT_005e00cc + iVar6 * 8);
  iVar4 = (int)*(short *)(&DAT_007999e0 + param_1 * 0x108) + *(int *)(&DAT_005e00d0 + iVar6 * 8);
  iVar6 = *(int *)(&DAT_005e014c + (iVar6 + (DAT_00bca2e0 / 2) * 4) * 4);
  puVar2 = &DAT_005df9bc;
  local_24 = iVar8;
  local_20 = iVar4;
  do {
    local_18 = 0xd;
    iVar5 = iVar8;
    param_1 = iVar4;
    do {
      iVar7 = iVar5;
      iVar9 = local_20;
      if ((uVar1 != 0) && (iVar7 = local_24, iVar9 = param_1, uVar1 != 2)) {
        if (uVar1 == 4) {
          iVar7 = param_1 + (iVar8 - iVar4);
          iVar9 = local_24 + (iVar4 - iVar8);
        }
        else {
          iVar7 = local_20 + (iVar8 - iVar4);
          iVar9 = (iVar4 - iVar8) + iVar5;
        }
      }
      if (puVar2[-3] != 0) {
        if (iVar6 == 0) {
          uVar3 = puVar2[-2];
        }
        else if (iVar6 == 2) {
          uVar3 = puVar2[-1];
        }
        else if (iVar6 == 4) {
          uVar3 = *puVar2;
        }
        else {
          uVar3 = puVar2[1];
        }
        FUN_004c0860(iVar7,iVar9,puVar2[-3],uVar3);
        FUN_004746d0(iVar7,iVar9,1);
      }
      puVar2 = puVar2 + 5;
      param_1 = param_1 + -1;
      iVar5 = iVar5 + 1;
      local_18 = local_18 + -1;
    } while (local_18 != 0);
    local_24 = local_24 + -1;
    local_20 = local_20 + 1;
  } while ((int)puVar2 < 0x5e00d8);
  return;
}
